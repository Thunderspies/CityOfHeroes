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
		Sound Clockwork_WalkServoSmall_01 100 100 .15
		Sound Clockwork_WalkServoSmall_02 100 100 .15
		Sound Clockwork_WalkServoSmall_03 100 100 .15
		Sound Clockwork_WalkServoSmall_04 100 100 .15
		Sound Clockwork_WalkServoSmall_05 100 100 .15
		Sound Clockwork_WalkServoSmall_06 100 100 .15
	End

End       

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		Sound Clockwork_WalkFFFemale_01 100 100 .25
		Sound Clockwork_WalkFFFemale_02 100 100 .25
		Sound Clockwork_WalkFFFemale_03 100 100 .25
		Sound Clockwork_WalkFFFemale_04 100 100 .25
		Sound Clockwork_WalkFFFemale_05 100 100 .25
		Sound Clockwork_WalkFFFemale_05 100 100 .25
		Sound Clockwork_WalkFFFemale_07 100 100 .25
	End

End
