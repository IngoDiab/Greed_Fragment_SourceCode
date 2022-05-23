#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Greed/Utils/Action/Action.h"
#include "G_NemesisDestruct.generated.h"

UCLASS()
class GREED_API AG_NemesisDestruct : public AActor
{
	GENERATED_BODY()

	Action<> mOnEndDestruction = Action<>();
	
public:
	FORCEINLINE Action<>& OnEndDestruction() { return mOnEndDestruction; };

public:	
	AG_NemesisDestruct();
	virtual void BeginDestroy() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "End Destruction")
	void EndDestruct();
};
