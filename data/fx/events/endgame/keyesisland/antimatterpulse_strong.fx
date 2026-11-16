#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 210

Flags	DontSuppress

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Start
		Bhvr	:Camera_Shake_Settle.bhvr
		Sound Antimatter_Pulse_Strong_01 200 200 .9
		Lifespan 5
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Green_Subtractive.part
		Part	:Filter_Green.part
		Part	:Clouds_Border.part


		Lifespan 210
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	AshAnchor
		Type	Start
		CAMERASPACE
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Ashes_Quick.part
		Part	:Embers.part
		Lifespan	100
	End
End

#############################################################

End
