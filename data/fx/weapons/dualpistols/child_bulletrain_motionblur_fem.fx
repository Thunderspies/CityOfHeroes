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
			StartColor		48 48 128
		End
		Anim	FX_MotionBlur_Fem
		Lifespan 35
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
			StartColor		36 36 96
		End
		Anim	FX_MotionBlur_Fem
		Lifespan 34
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
			StartColor		32 32 64
		End
		Anim	FX_MotionBlur_Fem
		Lifespan 33
	End
End

#############################################################

End