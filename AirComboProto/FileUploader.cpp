// Fill out your copyright notice in the Description page of Project Settings.

#include "FileUploader.h"
#include "AirComboProto.h"


UFileUploader::UFileUploader():
	FileUrl(TEXT(""))
	, FileSavePath(TEXT(""))
{
}

UFileUploader::~UFileUploader()
{
}

UFileUploader* UFileUploader::MakeUploader()
{
	UFileUploader* Uploader = NewObject<UFileUploader>();
	return Uploader;
}

UFileUploader* UFileUploader::UploadFile(const FString& Url, FString SavePath)
{

	FileUrl = Url;
	FileSavePath = SavePath;

	// file name
	int32 LastSlashPos;
	FileSavePath.FindLastChar('\\', LastSlashPos);
	FString FileName = FileSavePath.RightChop(LastSlashPos + 1);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FileName);
	// get data
	TArray<uint8> UpFileRawData;
	FFileHelper::LoadFileToArray(UpFileRawData, *FileSavePath);

	// prepare json data
	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonString);
	
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("FileToUpload", FileName);
	JsonWriter->WriteValue("ImageData", FBase64::Encode(UpFileRawData));
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	//HttpRequest->SetURL(Url);
	HttpRequest->SetURL("http://54.69.31.251/replayConvert.php");
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UFileUploader::OnReady);
	HttpRequest->OnRequestProgress().BindUObject(this, &UFileUploader::OnProgress_Internal);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, JsonString);
	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

void UFileUploader::OnReady(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();
	
	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnResult.Broadcast(EUploadResult::Success);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Response->GetContentAsString());
		/*
		// SAVE FILE
		//IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// create save directory if not existent
		FString Path, Filename, Extension;
		FPaths::Split(FileSavePath, Path, Filename, Extension);
		if (!PlatformFile.DirectoryExists(*Path))
		{
			if(!PlatformFile.CreateDirectoryTree(*Path))
			{
				OnResult.Broadcast(EUploadResult::DirectoryCreationFailed);
				return;
			}
		}

		// open/create the file
		IFileHandle* FileHandle = PlatformFile.OpenWrite(*FileSavePath);
		if (FileHandle)
		{
			// write the file
			FileHandle->Write(Response->GetContent().GetData(), Response->GetContentLength());
			// Close the file
			delete FileHandle;

			OnResult.Broadcast(EUploadResult::Success);
		}
		else
		{
			OnResult.Broadcast(EUploadResult::SaveFailed);
		}
		*/
	}
	else
	{
		OnResult.Broadcast(EUploadResult::UploadFailed);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Response->GetContentAsString());
	}
	
}

void UFileUploader::OnProgress_Internal(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	int32 FullSize = Request->GetContentLength();
	OnProgress.Broadcast(BytesSent, BytesReceived, FullSize);
}