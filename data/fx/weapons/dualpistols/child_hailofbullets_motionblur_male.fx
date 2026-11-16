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
		BhvrOverride
			StartColor		48 48 128
		End
		Anim	FX_MotionBlur_Male
		Lifespan 105
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		36 36 96
		End
		Anim	FX_MotionBlur_Male
		Lifespan 104
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur1.bhvr
		BhvrOverride
			StartColor		24 24 64
		End
		Anim	FX_MotionBlur_Male
		Lifespan 102
	End
End

#############################################################

End