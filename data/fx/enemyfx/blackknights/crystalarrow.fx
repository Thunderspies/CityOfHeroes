#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

	LifeSpan 300

	ClampMinScale 0.875 0.875 0.875
	ClampMaxScale 1.125 1.125 1.125

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BlackKnights_CrystalArrow 100 100 .8
	End
End

	Condition
		On Time
		Time 5

		Event
			At WepR
			Type Local
			Bhvr WEAPONS\Bow\Archery\HandRotateFlame.bhvr
			Geom CrystalArrow
			BhvrOverride
				StartColor 180 200 255
			End
			#Sound arrowout2 50 50 0.25
			LifeSpan 16
		End

	End

	Condition
		On Time
		Time 10

		Event
			At WepR
			Type Local
			Bhvr WEAPONS\Bow\Archery\HandRotate.bhvr
			Geom CrystalArrow
			BhvrOverride
				StartColor 180 200 255
			End
			LifeSpan 12
		End

	End

	Condition
		On Time
		Time 22

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :FXArrowProjectile.bhvr
			Part :ColdSmoke_Trail01.part
			Part :ColdSmoke_Trail02.part
			Geom CrystalArrow
		End

		Event
			At Prime
			Type Local
			BhvrOverride
				PositionOffset 0 0 2
			End
			Part :Tracer_Trail.part
			Part ENEMYFX\Praetorian\Talons\IceBoltMistTrail2.part
			Part ENEMYFX\Praetorian\Talons\EnergyProjectileStreaks.part
			Part :Comet_Sm.part
		End

	End

	Condition
		On PrimeHit

#		Event
#			At T_Chest
#			Type Start
#			#ChildFx :CrystalArrow_Hit.fx
#			Bhvr WEAPONS\Bow\Archery\StunArrowHit.bhvr
#			BhvrOverride
#				StartColor 180 200 255
#			End
#			Geom CrystalArrow
#			LifeSpan 70
#		End

		Event
			Type Destroy
			EName prime
		End

	End

End
