#
FxInfo
	Lifespan 150

###########################################################

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Sound BlackKnights_NetherBlade_Projectile 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName KeetsiePwns1
			Flags Oneatatime
			While WEAPON
			BhvrOverride
				Alpha 1
			End
			Geom CoL_Staff_01
		End

		Event
			AltPiv 1
			At KeetsiePwns1
			Type Local
			EName KeetsiePwns2
			BhvrOverride
				PositionOffset 0 0 .5
			End
		End

		Event
			At KeetsiePwns2
			Type Local
			Ename KeetsiePwns3
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SwordCharge.part
			Lifespan 14
		End

	End

	Condition
		On Time
		Time 18

		Event
			EName KeetsiePwns4
			AltPiv 1
			At KeetsiePwns1
			Type Local
			BhvrOverride
				PositionOffset 0 0 2
			End
			Bhvr Behaviors/GenericParticleFade.bhvr
		#	Part ENEMYFX\Carnival_Of_Light\ProjectileInitFlash.part
		End

	End

	Condition
		On Time
		Time 24

		Event
			At WepR
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr behaviors\Braz\Projectile_Fast_01.bhvr
			Part :Sword_Projectile_Solid.part
			Part :Sword_Projectile_Wisp.Part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName Prime2
		End

		Event
			Type Destroy
			EName Prime3
		End

	End

	Condition
		On Time
		Time 50

		Event
			Type Destroy
			EName Hand1
		End

		Event
			Type Destroy
			EName Hand2
		End

	End

End
