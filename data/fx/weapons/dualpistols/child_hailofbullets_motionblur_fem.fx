#############################################################
## HailOfBullets_Attack.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		Anim	FX_MotionBlur_Fem
		Lifespan 65
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur2.bhvr
		Anim	FX_MotionBlur_Fem
		Lifespan 63
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur3.bhvr
		Anim	FX_MotionBlur_Fem
		Lifespan 57
	End
End

#############################################################

End