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
			Anim	FX_Backpack_Mecha_01A
			BhvrOverride
				Scale			1.22 1.22 1.22
				PositionOffset		0 0 -0.2
				PYRrotate 		-10 0 0
	# 			PYRrotateJitter 	0 0 0
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
				PositionOffset 3.6 0.5 -2.8
				scale .1 .1 .1
			End
			#geom geo_FX_TestSphere
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
				PositionOffset -3.6 0.5 -2.8
				scale .1 .1 .1
			End
			#geom geo_FX_TestSphere
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
				PositionOffset -3.6 0.5 -2.8
				scale .1 .1 .1
			End
			#geom geo_FX_TestSphere
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
				PositionOffset 3.6 0.5 -2.8
				scale .1 .1 .1
			End
			#geom geo_FX_TestSphere
			ChildFX :Wing_AirStreams.fx
		End
	End

	Condition
		On TriggerBits
		TriggerBits FLY
		DoMany 1

		Event
			At MECHAANCHOR
			Type Local
			EName ThrusterFireLeft
			Flags OneAtATime
			While FLY
			BhvrOverride
				PositionOffset -0.13 -0.4 -0.42
				PyrRotate -20 0 8
				FadeoutLength 20
			End
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire.part
		End

		Event
			At MECHAANCHOR
			Type Local
			EName ThrusterFireRight
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				FadeoutLength 20
				PositionOffset 0.13 -0.4 -0.42
				PyrRotate -20 0 -8
			End
			Part Costumes\Backpacks\Mecha_01\Idle_Fire_Add.part
			Part Costumes\Backpacks\Mecha_01\Idle_Fire.part
		End

		Event
			At MECHAANCHOR
			Type Local
			EName ThrusterGlowLeft
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.15 -0.4 -0.43
				PyrRotate -65 0 20
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End


		Event
			At MECHAANCHOR
			Type Local
			EName ThrusterGlowRight
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.15 -0.4 -0.43
				PyrRotate -65 0 -20
			End
			Part Costumes\Backpacks\Mecha_01\Thruster_Glow.part
		End

		Event
			At MECHAANCHOR
			Type Local
			EName ThrusterMain
			Flags OneAtATime
			While FLY
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Scale 1.15 1 1.01
				PositionOffset 0 0.02 0
				#PyrRotate -8 0 0
				StartColor 255 245 200
			End
			Geom FX_Mecha_ThrusterGlow_01A
		End

		Event
			At MECHAANCHOR
			Type Local
			EName ThrusterMain2
			Flags OneAtATime
			While FLY
			#Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Scale 1.15 1 1.01
				PositionOffset 0 0.02 0
				#PyrRotate -8 0 0
				StartColor 255 245 200
			End
			Geom FX_Mecha_ThrusterGlow_01A
		End
	End

End