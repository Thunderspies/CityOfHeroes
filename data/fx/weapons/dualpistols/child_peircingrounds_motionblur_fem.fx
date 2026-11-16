#############################################################
## Child_BulletRain_MotionBlur_Male.fx
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
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur_B1.bhvr
		BhvrOverride
			StartColor		32 32 64
		End
		Anim	FX_MotionBlur_Fem
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur_B1.bhvr
		BhvrOverride
			StartColor		24 24 48
		End
		Anim	FX_MotionBlur_Fem
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	Root
		Bhvr	WEAPONS\DualPistols\FX_DualPistols_MBlur_B1.bhvr
		BhvrOverride
			StartColor		16 16 32
		End
		Anim	FX_MotionBlur_Fem
		Lifespan 40
	End
End

#############################################################

End