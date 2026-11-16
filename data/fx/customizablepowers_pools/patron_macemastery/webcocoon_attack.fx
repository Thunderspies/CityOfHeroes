#                                                          
FxInfo

	LifeSpan 200

	ClampMaxScale 1 1 1

###########################################################

	Condition
		On Time
		Time 36

		Event
			At WepR
			Type Local
			EName Blast
			Geom heavyMace
		End

		Event
			AltPiv 2
			At Blast
			Type Start
			EName tracer
			Bhvr Behaviors\ShotGun.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_Fx_GunBurst
			LifeSpan 2
		End

		Event
			AltPiv 2
			At Blast
			Type Start
			EName tracer2
			Part :EmissionPoint.part
			Part :BlastStreaks.part
			Part :Gunfire_ShotgunMuzzflash.part
			Part :Gunfire_Shotgunsmoke.part
			Part :Gunfire_ShotgunMuzzflashSmall.part
			Sound grenade4 80.0 30.0 0.8
			LifeSpan 10
		End

		Event
			AltPiv 2
			At Blast
			Type Start
			EName Prime
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors\MorterLobbprojectile2.bhvr
			Part :BeanBagDust2.part
			Geom grenade
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Root
			Type Local
			EName RadiusFxScale
			Bhvr powers\firecontrol\Inferno.bhvr
		End

		Event
			At RadiusFxScale
			Type Start
			EName Prime1
			Bhvr WEAPONs\Bow\TrickArrow\stickyBubble.bhvr
			Part :StickyYellow2.part
			Part :StickyGlows.part
			Sound webgrexplo 70 60 0.88
			LifeSpan 30
		End

		Event
			Type Destroy
			EName Prime
		End

	End

End
