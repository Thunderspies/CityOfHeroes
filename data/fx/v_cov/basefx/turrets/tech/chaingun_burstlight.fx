#########################################################
##	
##

FxInfo
LifeSpan  300

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitLeft
		Type	Local	
		At	Waist
		
		bhvr	V_COV\BaseFx\turrets\Tech\Chaingun_B_EmitLeft.bhvr
#		Geom	Testing_TargetB
	End

	Event
		EName	EmitRight
		Type	Local	
		At	Waist
		BhvrOverride
			behavior
			PositionOffset		1.151 .2 3.5
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Chaingun_B_EmitLeft.bhvr
#		Geom	Testing_TargetB
	End

######################################################################################################
########################################  Emitter Shells  ############################################
######################################################################################################

	Event
		EName	ShellsEmitL
		Type	Local	
		At	Waist

		bhvr	V_COV\BaseFx\turrets\Tech\Chaingun_B_ShellEmitLeft.bhvr
#		Geom	Testing_TargetB
	End

	Event
		EName	ShellsEmitR
		Type	Local	
		At	Waist

		bhvr	V_COV\BaseFx\turrets\Tech\Chaingun_B_ShellEmitRight.bhvr
#		Geom	Testing_TargetB
	End


End

#####################################################################################################
#####################################  Emitting Shells Left  #########################################
######################################################################################################		

Condition
	On 	Time
	Time 	31

	Event
		EName	ShotSound
		Type	Local
		At	EmitRight
		
		Sound Assault2 120.0 120.0 1.0
	End


	Event
		ENAME   ShellsLeft
		Type	Local 
		At	ShellsEmitL
		BhvrOverride
			PositionOffset	0 0 0
		End

		Bhvr	Behaviors/ChainGunBustMedium.bhvr
		Part1	WEAPONFX\Gunfire_AssaultShell.Part
		
		Lifespan 14
	End

######################################################################################################
############################################  Firing Left  ###########################################
######################################################################################################	

	Event	
		EName	CannonGEO
		Type	Local
		At	Waist
		bhvr	V_COV\BaseFx\turrets\Tech\Chaingun_B_BarrelLeft.bhvr
		BhvrOverride
			behavior
			PositionOffset		-1.15 .027 1.58 #1.15 .027 1.944
			Scale			.875 .875 .91
		End
		Geom	GEO_Barrel__WorldAnim
#		Geom	Testing_TargetB
		Lifespan 16
	End

	Event
		EName 	Blast1
		Type	local
		At	EmitLeft

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

	Event
		EName 	tracer
		Type	local
		At	EmitLeft

		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan 14
	End

End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Blast2
		Type	local
		At	EmitLeft

#		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End


Condition
	On 	Time
	Time 	35

	Event
		EName 	Blast3
		Type	local
		At	EmitLeft

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End

Condition
	On 	Time
	Time 	38

	Event
		EName 	Blast4
		Type	local
		At	EmitLeft

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End

Condition
	On 	Time
	Time 	41

	Event
		EName 	Blast5
		Type	local
		At	EmitLeft

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End

#####################################################################################################
####################################  Emitting Shells Right  ########################################
#####################################################################################################		

Condition
	On 	Time
	Time 	33

	Event
		ENAME   ShellsRight
		Type	Local 
		At	ShellsEmitR
		BhvrOverride
			PositionOffset	0 0 0
		End

		Bhvr	Behaviors/ChainGunBustMedium.bhvr
		Part1	WEAPONFX\Gunfire_AssaultShell.Part
		Lifespan 14
	End

#####################################################################################################
###########################################  Firing Right ###########################################
#####################################################################################################	

	Event	
		EName	CannonGEO_Right
		Type	Local
		At	Waist
		BhvrOverride
			behavior
			PositionOffset		1.15 .027 1.58 #1.15 .027 1.944
			Spin			0 0 -5
			Scale			.875 .875 .91
		End
		
		bhvr	V_COV\BaseFx\turrets\Tech\Chaingun_B_BarrelLeft.bhvr
		Geom	GEO_Barrel__WorldAnim
#		Geom	Testing_TargetB
		Lifespan 16
	End

	Event
		EName 	Blast1R
		Type	local
		At	EmitRight

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

	Event
		EName 	tracerR
		Type	local
		At	EmitRight

		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan 14
	End

End

Condition
	On 	Time
	Time 	34

	Event
		EName 	Blast2R
		Type	local
		At	EmitRight

#		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End


Condition
	On 	Time
	Time 	37

	Event
		EName 	Blast3R
		Type	local
		At	EmitRight

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName 	Blast4R
		Type	local
		At	EmitRight

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End

Condition
	On 	Time
	Time 	43

	Event
		EName 	Blast5R
		Type	local
		At	EmitRight

		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan 3
	End

End


#################################


End				