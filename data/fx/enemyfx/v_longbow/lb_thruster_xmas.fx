#########################################################
##	Fireweapon
########################################################
FxInfo

Flags InheritAlpha DontSuppress

########################################################

Condition

	Event
		Type	local
		At	Chest
		Sound xmasdrone_loop 70 70 .62
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 430

	End

End

Condition

	Event
		Type	local
		At	Chest
		Sound thruster_loop 70 70 .2
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 400

	End

End




Condition
	On 	Time
	Time 	0

	Event
		EName 	Emitter
		Type	local
		At	Chest

		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterMid.bhvr

	End

End

#####################################################################################
################################### center jet  #####################################
#####################################################################################

Condition
	On	Time
	Time	1

	Event
		EName 	JetR
		Type	local
		At	Emitter
				BhvrOverride
			Behavior
			PositionOffset 0 -.4 -.24
		End

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit_XMAS.part
		Part2	ENEMYFX\V_LongBow\LB_P_ThrustFlareCore_XMAS.part
		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare_XMAS.part

	End

	Event
		EName	FlareRim
		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -.4 -.24
		End

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRings_XMAS.part
	End

	Event
		EName	FlareRim2
		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -0.5 -0.27
		End

		Part1	ENEMYFX\V_LongBow\LB_P_Smoke_XMAS.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -0.5 -0.25
		End
		Part1	:LB_Thruster_Snowburst.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -0.3 -0.15
		End
		Part1	:LB_Thruster_Sparkle01.part
	End
	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -0.3 -0.15
		End
		Part1	:LB_Thruster_Sparkle02.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -0.3 -0.15
		End
		Part1	:LB_Thruster_Sparkle03.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 0.5 -0.1
		End
		Part1	:LB_Thruster_Sparkle_Static.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset -0.2 0.25 -0.1
		End
		Part1	:LB_Thruster_Sparkle_Static_Green.part
		Part2	:LB_Thruster_Sparkle_Static_Red.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0.2 0.25 -0.1
		End
		Part1	:LB_Thruster_Sparkle_Static_Green.part
		Part2	:LB_Thruster_Sparkle_Static_Red.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset -0.65 0.25 0.5
		End
		Part1	:LB_Thruster_Sparkle_Static_Green.part
		Part2	:LB_Thruster_Sparkle_Static_Red.part
	End

	Event

		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0.65 0.25 0.5
		End
		Part1	:LB_Thruster_Sparkle_Static_Green.part
		Part2	:LB_Thruster_Sparkle_Static_Red.part
	End

End

