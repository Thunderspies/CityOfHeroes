#############################################################
## Costume Anim Entity - Backpack
#############################################################

FxInfo

Flags InheritAlpha DontSuppress //InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	MECHAANCHOR
		Type	Local
		At	Back
		Anim	FX_Backpack_Mecha_01B
		BhvrOverride
			Scale			0.9 0.9 0.9
			PositionOffset		0 -0.03 0.09
			PYRrotate 		-6 0 0
# 			PYRrotateJitter 	0 0 0
		End
	End

		Event
			Ename	MECHAANCHOR1
			Type	Local
			At	MECHAANCHOR
			Geom FX_Backpack_Mecha01B_Offsets
			BhvrOverride
				Alpha 1
		End
	End

End

	Condition
		On TriggerBits
		TriggerBits FLY
		DoMany 1

		Event
			At MECHAANCHOR1
			Type Local
			EName ThrusterFireLeft
			Flags OneAtATime
			While FLY
			Altpiv 1
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.07 0
			End
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big.part
		End

		Event
			At MECHAANCHOR1
			Type Local
			EName ThrusterGlowLeft
			Flags OneAtATime
			While FLY
			Altpiv 1
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.07 0
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow2.part
			#Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End

		Event
			At MECHAANCHOR1
			Type Local
			EName ThrusterFireRight
			Flags OneAtATime
			While FLY
			Altpiv 2
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.07 0
			End
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Big.part
		End

		Event
			At MECHAANCHOR1
			Type Local
			EName ThrusterGlowRight
			Flags OneAtATime
			While FLY
			Altpiv 2
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.07 0
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow2.part
			#Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End

	End

End