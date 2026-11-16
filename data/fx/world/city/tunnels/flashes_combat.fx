#############################################################
## Smoke_StormCloud.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	StartPositOnly
		At	Origin
		Part	:Smoke_Pillar_Blobby.part
		Part	:Smoke_Pillar.part
	End

	Event
		EName 	Prime
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -50.0 0.0
		End
		Part	:Ashes.part
		Part	:Embers.part
	End

	Event
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0.0 140.0 0.0
		End
		Part	:Smoke_Cloud_Center.part
		Part	:Smoke_Cloud_Center_Blobby.part
	End
End


Condition
	On	Cycle
	Time	10
	Chance	0.4

	Event
		Type	StartPositOnly
		At	Prime
		BhvrOverride
			PositionOffset	0.0 140.0 0.0
		End
		Part	:Flashes_Cloud.part
		Part	:Flashes_Cloud_Highlights.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.35
	Event
		Type	StartPositOnly
		At	Prime
		BhvrOverride
			PositionOffset	0.0 140.0 0.0
		End
		Part	:Flashes_Cloud.part
		Lifespan 1
	End
End