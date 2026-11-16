#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

	LifeSpan 300

	ClampMinScale 0.875 0.875 0.875
	ClampMaxScale 1.125 1.125 1.125

###########################################################

	Condition
		On Time
		Time 5

		Event
			At WepR
			Type Local
			Bhvr WEAPONS\Bow\Archery\HandRotateFlame.bhvr
			Geom ObsidianArrow
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
			Geom ObsidianArrow
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
			Part ENEMYFX\BlackKnights\DarkSmoke_Trail01.part
			Part ENEMYFX\BlackKnights\DarkPatch_Trail03.part
			Part ENEMYFX\BlackKnights\DarkComet_Sm.part
			Part ENEMYFX\BlackKnights\DarkEnergyProjectileStreaks.part
			Part ENEMYFX\BlackKnights\DarkTracer_Trail.part
			Part ENEMYFX\BlackKnights\SpiritSmoke_Trail.part
			Geom ObsidianArrow
		End

		Event
			At WepL
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr :FXArrowProjectile_Dummy.bhvr
			BhvrOverride
				TrackRate 1.8
			End
		End

		Event
			At Prime2
			Type Local
			ChildFx :Ghost_Child_01.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

		Event
			At Prime2
			Type Local
			ChildFx :Ghost_Child_02.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

		Event
			At Prime2
			Type Local
			ChildFx :Ghost_Child_03.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

		Event
			At Prime2
			Type Local
			ChildFx :Ghost_Child_04.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

	End

	Condition
		On Time
		Time 28

		Event
			At WepL
			Type Start
			EName Prime3
			LookAt Target
			Magnet Target
			Bhvr :FXArrowProjectile_Dummy.bhvr
			BhvrOverride
				TrackRate 1
			End
		End

		Event
			At Prime3
			Type Local
			ChildFx :Ghost_Child_01.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

		Event
			At Prime3
			Type Local
			ChildFx :Ghost_Child_02.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

		Event
			At Prime3
			Type Local
			ChildFx :Ghost_Child_03.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

		Event
			At Prime3
			Type Local
			ChildFx :Ghost_Child_04.fx
			BhvrOverride
				StartJitter 0 0 1
				SpinJitter 0 0 0.1
				Spin 0 0 0.2
			End
		End

	End

	Condition
		On PrimeHit

#		Event
#			At T_Chest
#			Type Start
#			#ChildFx :SpiritArrow_Hit.fx
#		End

		Event
			Type Destroy
			EName prime
		End

	End

	Condition
		On Time
		Time 100

		Event
			Type Destroy
			EName prime2
		End

		Event
			Type Destroy
			EName prime3
		End

	End

	#Condition
		#On Time
		#Time 0

		#Event
			#At Root
			#Type Local
			#BhvrOverride
				#PositionOffset 0.5 5 2
				#StartColor 220 240 255
			#End
			#Geom ObsidianArrow
		#End

		#Event
			#At Root
			#Type Local
			#BhvrOverride
				#PositionOffset -0.5 5 2
			#End
			#Geom ObsidianArrow
		#End

	#End

End
