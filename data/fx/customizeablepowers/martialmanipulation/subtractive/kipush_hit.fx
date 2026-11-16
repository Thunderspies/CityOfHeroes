#
FxInfo

	LifeSpan 60


###########################################################

	Condition
		On Time
		Time 0

		Event
			At T_Chest
			Type Local
			EName ShotAnchor
			LookAt ShotAnchorTarget
			PMagnet ShotAnchorTarget
			POther ShotAnchorTarget
#			Part CustomizeablePowers\MartialManipulation\Subtractive\ForceArch2.part
#			Part CustomizeablePowers\MartialManipulation\Subtractive\ForceArch.part
		End

		Event
			At Chest
			Type Local
			EName ShotAnchorTarget
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -1 0
			End
			LookAt ShotAnchor
			PMagnet ShotAnchor
			POther ShotAnchor
			Part CustomizeablePowers\MartialManipulation\Subtractive\ForceArch2.part
			Part CustomizeablePowers\MartialManipulation\Subtractive\ForceArch.part
		End

		Event
			At Chest
			Type Local
			EName ShockwaveAnchor
			Part CustomizeablePowers\MartialManipulation\Subtractive\Flash_Streaks_Small.part
			Sound KM_BodyBlow_Hit_01 200 50 1
			LifeSpan 5
		End

		Event
			At Chest
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 60 30 30
				PyrRotateJitter 10 0 10
			End
			Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Burst_LocalFacing.part
			Part CustomizeablePowers\MartialManipulation\Subtractive\Flash_Core.part
			Part CustomizeablePowers\MartialManipulation\Subtractive\Flash_Glow_Small.part
			Sound PP11 200 100 0.7
			LifeSpan 15
			LifeSpanJitter 5
		End

		Event
			At Chest
			Type PositOnly
			BhvrOverride
				PyrRotate 100 0 -30
				PyrRotateJitter 10 0 10
			End
			Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Burst_LocalFacing_Small.part
			LifeSpan 15
			LifeSpanJitter 5
		End

		Event
			At Chest
			Type PositOnly
			BhvrOverride
				PyrRotate 0 120 0
				PyrRotateJitter 20 0 20
			End
			Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Burst_LocalFacing_Small.part
			LifeSpan 15
			LifeSpanJitter 5
		End

	End

	Condition
		On Cycle
		Time 2

		Event
			At ShockwaveAnchor
			Type Local
			BhvrOverride
				PyrRotateJitter 180 180 180
			End
			Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Burst_LocalFacing_Small.part
			LifeSpan 5
		End

	End

	#Condition
		#On Time
		#Time 70

		#Event
			#At Chest
			#Type Local
			#Bhvr behaviors/GenericParticleFade.bhvr
			#BhvrOverride
				#PyrRotate 60 30 30
				#PyrRotateJitter 10 0 10
			#End
			#Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Burst_LocalFacing.part
			#Part CustomizeablePowers\MartialManipulation\Subtractive\Flash_Core.part
			#Part CustomizeablePowers\MartialManipulation\Subtractive\Flash_Glow_Small.part
			#Sound PP11 200 100 0.7
			#LifeSpan 15
			#LifeSpanJitter 5
		#End

	#End

End
