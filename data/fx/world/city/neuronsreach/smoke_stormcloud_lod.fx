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

		#Part	:Smoke_Pillar_Blobby2.part
		Part	:Smoke_Pillar2.part
		Part	:Energy_Blobs.part
	#	Part	:Energy_Steam.part
		Part	:Energy_Splash.part
	#	Part	:Energy_Blobs_Splash.part
		Sound Neutropolis_SmokeStack01_Loop 200 200 .7
	End

	Event
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0.0 140.0 0.0
		End
	#	Part	:Smoke_Cloud_Center.part
		Part	:Smoke_Cloud_Center_Blobby.part
	End
End

#############################################################

End