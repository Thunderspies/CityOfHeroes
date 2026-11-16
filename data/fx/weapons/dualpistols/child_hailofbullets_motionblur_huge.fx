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
		Anim	FX_MotionBlur_Huge
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
		BhvrOverride
			StartColor		36 36 96
		End
		Anim	FX_MotionBlur_Huge
		Lifespan 64
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur3.bhvr
		BhvrOverride
			StartColor		24 24 64
		End
		Anim	FX_MotionBlur_Huge
		Lifespan 63
	End
End

#############################################################

End