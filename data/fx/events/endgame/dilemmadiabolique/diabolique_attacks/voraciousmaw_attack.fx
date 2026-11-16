#############################################################
## BitterIceBolt.fx
#############################################################

FxInfo
Lifespan 90

## HANDS ###########################################################

#Condition
#	On 	Time
#	Time	0
#	Event
#		Type	Local
#		At	Root
#		Sound 	BPS_VoraciousMaw_VOX_01 100 100 .8
#	End
#End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_MotAbsorbtion_01 200 200 .8
		Sound 	DA_MotAbsorbtion_02 200 200 .8
		Sound 	DA_MotAbsorbtion_03 200 200 .8
	End
End


Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	BP_VoraciousMaw_BigMouth_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
			EName	MawAchor
			At Root
			Type Local
			Part	:VoraciousMaw_TarPatch_Core.part

	End
End

Condition
	On	Time
	Time	0

	Event
			Anim FX_Maw_Tintable
			At Root
			Type Local
			BhvrOverride
			#	StartColor	1 1 1
				PositionOffset 0 -60 0
				PYRRotateJitter	0 180 0
				Scale 0.7 0.4 0.7
				AnimScale 0.1
			End
			LifeSpan 80
	End

End

#############################################################

End