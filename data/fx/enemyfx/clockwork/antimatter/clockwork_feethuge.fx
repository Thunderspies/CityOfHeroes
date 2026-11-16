#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 12

#############################################################
Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound Clockwork_WalkServo_01 100 100 .35
		Sound Clockwork_WalkServo_02 100 100 .35
		Sound Clockwork_WalkServo_03 100 100 .35
		Sound Clockwork_WalkServo_04 100 100 .35
		Sound Clockwork_WalkServo_05 100 100 .35
	End

End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		Sound Clockwork_WalkFFHuge_01 100 100 .25
		Sound Clockwork_WalkFFHuge_02 100 100 .25
		Sound Clockwork_WalkFFHuge_03 100 100 .25
		Sound Clockwork_WalkFFHuge_04 100 100 .25
		Sound Clockwork_WalkFFHuge_05 100 100 .25
		Sound Clockwork_WalkFFHuge_06 100 100 .25
		Sound Clockwork_WalkFFHuge_07 100 100 .25
	End

End
