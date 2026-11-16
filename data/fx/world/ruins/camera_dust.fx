#############################################################
## CAMERA_Dust.fx
#############################################################

FxInfo

###########################################################

Condition
	On Time
	Time 0

	Event
		EName Dust
		Type Start
		CAMERASPACE
		Bhvr behaviors\GenericParticleFade.bhvr
		Part :CAMERA_Dust.part
	End
End

Condition
	On 	Death

	Event
		EName	Dust
		Type 	Destroy
	End
End

###########################################################

End