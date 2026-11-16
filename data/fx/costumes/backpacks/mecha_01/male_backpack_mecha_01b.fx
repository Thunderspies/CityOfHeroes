#                                                          
FxInfo

	Flags InheritAlpha DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			Anim FX_Backpack_Mecha_01B
			At Back
			Type Local
			BhvrOverride
				PositionOffset 0 0 -0.03
				PyrRotate -11 0 0
				Scale 1.15 1.15 1.15
			End
		End

	End

	Condition
		On TriggerBits
		TriggerBits FLY
		DoMany 1

		Event
			At Back
			Type Local
			EName ThrusterFireLeft
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.78 -0.5 -0.915
				PyrRotate -11 0 0
				FadeOutLength 20
			End
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big.part
		End

		Event
			At Back
			Type Local
			EName ThrusterGlowLeft
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.78 -0.51 -0.915
				PyrRotate -11 0 0
				FadeOutLength 20
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow2.part
			#Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End

		Event
			At Back
			Type Local
			EName ThrusterFireRight
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.78 -0.5 -0.915
				PyrRotate -11 0 0
				FadeOutLength 20
			End
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big.part
		End

		Event
			At Back
			Type Local
			EName ThrusterGlowRight
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.78 -0.51 -0.915
				PyrRotate -11 0 0
				FadeOutLength 20
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow2.part
			#Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End

	End

End
