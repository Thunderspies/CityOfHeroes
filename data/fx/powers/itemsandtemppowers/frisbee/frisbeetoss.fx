#
FxInfo

	LifeSpan 150


###########################################################
	Condition
		On Time
		Time 18
		Event
			Type	Local
			At	Origin
			Sound Basketball_Throw_01 30 30 .25
			Sound Basketball_Throw_02 30 30 .25
		End
	End

	Condition
		On Time
		Time 0

		Event
			At WepL
			Type Local
			EName StartAnchor
		End

		Event
			At T_WepR
			Type Local
			EName EndAnchor
			BhvrOverride
				PositionOffset 0.5 -0.05 0
				PyrRotate 0 0 90
			End
		End

	End

	Condition
		On Time
		Time 5

		Event
			At StartAnchor
			Type Local
			EName Windup
			BhvrOverride
				PyrRotate -90 0 0
				Scale .12 .12 .12
			End
			Geom Frisbee
			LifeSpan 11
		End

	End

	Condition
		On Time
		Time 11

		Event
			Type Start
			EName Prime
			At WepL
			LookAt T_WepR
			Magnet T_WepR
			Bhvr :Projectile_Frisbee.bhvr
			#Geom Geo_Fx_TestSphere
		End

	End

	Condition
		On Time
		Time 16

		Event
			Type Start
			EName Prime2
			At WepL
			LookAt T_WepR
			Magnet T_WepR
			Bhvr :Projectile_Frisbee.bhvr
		End

		Event
			Type Local
			At Prime2
			Childfx :Frisbee_Projectile_CHILD.fx
		End

	End

############################################

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End
	End

	Condition
		On Prime2Hit

		Event
			Type Destroy
			EName Prime2
		End

	End

End
