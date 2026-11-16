#############################################################
## PsychicLash_Windup.fx
#############################################################

FxInfo
Lifespan 38

#############################################################


Condition
	On	Time
	Time	0
#	Event
#		Type	Local
#		At	Root
#		Sound 	FS_PsychicLash_Attack_01 100 100 .8
#	End

	Event
		Type	Local
		At	LWing4
		Part	:Electricity_Attack.part
		Lifespan 20
	End

	Event
		Type	Local
		At	LWing4
		Part	:Smoke_Attack.part
		Part	:Glow_Attack.part
		Lifespan 10
	End
End

#############################################################

End