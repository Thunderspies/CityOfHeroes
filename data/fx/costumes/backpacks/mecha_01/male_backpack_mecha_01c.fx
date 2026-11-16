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
		Type	Local
		At	Back
		Anim	FX_Backpack_Mecha_01C
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 0.08 -0.07
			PYRrotate 		-2 0 0
		End
	End
End

	Condition
		On TriggerBits
		TriggerBits Fly
		DoMany 1

		Event
			At Back
			Type Local
			EName RThrusterFireForward
			Flags OneAtATime
			While FORWARD
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 4.0 0 -2.9
			End
			ChildFX :Wing_AirStreams.fx
		End

		Event
			At Back
			Type Local
			EName LThrusterFireForward
			Flags OneAtATime
			While FORWARD
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -4.0 0 -2.9
			End
			ChildFX :Wing_AirStreams.fx
		End

		Event
			At Back
			Type Local
			EName RThrusterFireUp
			Flags OneAtATime
			While JUMP
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 4.1 -2 -1.75
			End
			ChildFX :Wing_AirStreams.fx
		End

		Event
			At Back
			Type Local
			EName LThrusterFireUp
			Flags OneAtATime
			While JUMP
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -4.1 -2 -1.75
			End
			ChildFX :Wing_AirStreams.fx
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
			BhvrOverride
				PositionOffset -0.15 -0.3 -0.38
				PyrRotate -20 0 8
				FadeoutLength 20
			End
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire.part
		End

		Event
			At Back
			Type Local
			EName ThrusterFireRight
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				FadeoutLength 20
				PositionOffset 0.15 -0.3 -0.38
				PyrRotate -20 0 -8
			End
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire.part
		End

		Event
			At Back
			Type Local
			EName ThrusterGlowLeft
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.15 -0.3 -0.38
				PyrRotate -75 0 22
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End


		Event
			At Back
			Type Local
			EName ThrusterGlowRight
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.15 -0.3 -0.38
				PyrRotate -75 0 -22
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End

		Event
			At Back
			Type Local
			EName ThrusterMain
			Flags OneAtATime
			While FLY
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Scale 1.15 1 1.01
				PositionOffset 0 0.13 0.13
				PyrRotate -8 0 0
				StartColor 255 245 200
			End
			Geom FX_Mecha_ThrusterGlow_01A
		End

		Event
			At Back
			Type Local
			EName ThrusterMain2
			Flags OneAtATime
			While FLY
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Scale 1.15 1 1.01
				PositionOffset 0 0.13 0.13
				PyrRotate -8 0 0
				StartColor 255 245 200
			End
			Geom FX_Mecha_ThrusterGlow_01A
		End
	End


#############################################################

End