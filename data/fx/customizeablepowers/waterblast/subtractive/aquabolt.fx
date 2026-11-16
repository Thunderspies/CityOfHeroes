#
FxInfo

	LifeSpan 150
Input
	InpName	Hips
End

###########################################################
	Condition
		On 	Time
		Time	0
		Event
			Type	Local
			At	Root
			Sound 	WB_AquaBolt 100 100 .8
		End
	End

######################################################

	Condition
		On TriggerBits
		TriggerBits LEFT

#		Event
#			At WepL
#			Type Local
#			ChildFx :AquaBolt_CHILD_Left.fx
#			EName Punch_Left
#			Until RIGHT
#			BhvrOverride
#				PositionOffset 0 0 2
#			End
#		End

		Event
			Ename DrawIn
			At WepL
			Type Local
			Flags OneAtATime
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray.part
			Part :WaterBall_DrawIn_Spray2.part
			LifeSpan 10
		End

		Event
			Ename Anchor
			Type Local
			At WepL
			Flags OneAtATime
		End

	End

	Condition
		On TriggerBits
		TriggerBits RIGHT
		Domany 1

#		Event
#			At WepR
#			Type Local
#			ChildFx :AquaBolt_CHILD_right.fx
#			EName Punch_Right
#			Until LEFT
#		End

		Event
			Ename DrawIn
			At WepR
			Type Local
			Flags OneAtATime
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray.part
			Part :WaterBall_DrawIn_Spray2.part
			LifeSpan 10
		End

		Event
			Ename Anchor
			Type Local
			At WepR
			Flags OneAtATime
		End
	End


	Condition
		On Time
		Time 15

		Event
			At Anchor
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				InitialVelocity 0 0.1 0
				TrackRate 3
			End
			Part :WaterShot_Splash_Initial01.part
			Part :WaterShot_Splash_Initial02.part
			Part :WaterShot_Splash_Trail_01.part
			Part :WaterShot_Splash_Trail_02.part
			Part :WaterShot_Splash_Trail_03.part
			Part :WaterShot_Splash_Trail_04.part
			Part :WaterShot_Bullet_SM.part
			#Part :WaterShot_Drips.part
			Part :WaterShot_Drips2.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End


End
