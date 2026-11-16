#########################################################
##	Base Turrets Tech Energy Blast

FxInfo

LifeSpan	70

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Emit_L
		Type	Local
		At	Waist

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
#		Geom	Testing_TargetB
		Sound CABuildup 60.0 60.0 .4
	End

	Event	
		EName	Emit_R
		Type	Local
		At	Waist
		bhvrOverride
			behavior
			PositionOffset		1.191 0.05 3.2
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
#		Geom	Testing_TargetB
	End


######################################################################################################
##########################################  Emitter Rods  ############################################
######################################################################################################
	
	Event	
		EName	Rods_L
		Type	Local
		At	Waist
		bhvrOverride
			behavior
			PositionOffset		-1.170 -0.025 .875
			Scale			1 1 1
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rods_ParentL.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	Rods_R
		Type	Local
		At	Waist
		bhvrOverride
			behavior
			PositionOffset		1.170 -0.025 .875
			Scale			1 1 1
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rods_ParentL.bhvr
#		Geom	Testing_TargetB
	End
End

Condition	
	On	Time
	Time	1

	Event
		EName	Rods_L
		Type	Local
		At	Rods_L

		childFX V_COV\BaseFx\turrets\Tech\Energy_ChildRods.fx
	End

	Event
		EName	Rods_R
		Type	Local
		At	Rods_R

		childFX V_COV\BaseFx\turrets\Tech\Energy_ChildRods.fx
	End
End

######################################################################################################
##########################################  Charge Balls  ############################################
######################################################################################################

Condition
	On 	Time
	Time 	7
	
	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	POWERS\EnergyBlast\CometHandBubbles.part
		Part2	POWERS\EnergyBlast\CometHandBubblesWhite.part		
		PMagnet	WepR
		Lifespan 19
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	POWERS\EnergyBlast\CometHandBubbles.part
		Part2	POWERS\EnergyBlast\CometHandBubblesWhite.part
		PMagnet	WepL
		Lifespan 19
	End	
End

######################################################################################################
##########################################  XXXXXXXXXXXX  ############################################
######################################################################################################

Condition
	On	Time
	Time	19
	

	Event
		EName	coreL
		Type	local
		At	Emit_L
#		geom	MegaCore
		bhvr	behaviors/BlastScale3.bhvr
#		Sound	PowerBolt4 100.0 30.0 .7
		Lifespan 21
	End

	Event
		EName	coreR
		Type	local
		At	Emit_R
#		geom	MegaCore
		bhvr	behaviors/BlastScale3.bhvr
#		Sound	PowerBolt4 100.0 30.0 .7
		Lifespan 21
	End
End

######################################################################################################
##########################################  Barrel Flare  ############################################
######################################################################################################

Condition
	On 	Time
	Time 	18
	
	Event
		EName 	BarrelGlow_L
		Type	local
		At	Emit_L
		
		Part3	V_COV\BaseFx\turrets\Tech\Energy_P_StarWhite.part
		Part4	V_COV\BaseFx\turrets\Tech\Energy_P_CloudsWhite.part
		PMagnet	WepR
		Lifespan 15
	End

	Event
		EName 	BarrelGlow_R
		Type	local
		At	Emit_R
		
		Part3	V_COV\BaseFx\turrets\Tech\Energy_P_StarWhite.part
		Part4	V_COV\BaseFx\turrets\Tech\Energy_P_CloudsWhite.part
		PMagnet	WepL
		Lifespan 15
	End
End

######################################################################################################
#####################################  Prime Projectile Left  ########################################
######################################################################################################

Condition
	On	Time
	Time	25

	Event
		EName 	HandGlowFlashL
		Type	local
		At	Emit_L
		bhvrOverride
			behavior
			Scale		.5 .5 .5
		End

		Part1	POWERS\EnergyBlast\HandGlowWhite3.part
		Part2	POWERS\EnergyBlast\HandGlowEmber3.part
		Lifespan 3
				
		PMagnet	WepL
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Prime
		Type	start
		At	Emit_L
		
		Part2	POWERS\EnergyBlast\CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Sound WolfPackEnergyBlastHit 100.0 100.0 .7
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		
		Part1	POWERS\EnergyBlast\CometBubbles.part
		Part2	POWERS\EnergyBlast\CometBubblesBright.part
		Part3	POWERS\EnergyBlast\CometBubblesLocal.part
		Part4	POWERS\EnergyBlast\CometHead.part
		Part5	POWERS\EnergyBlast\EnergyCoreLocal.part
	End
End

######################################################################################################
###########################################  Gas Glow L  #############################################
######################################################################################################

Condition
	On	Time
	Time	27
	Event
		EName	cometPartsLocal
		Type	Local
		At	Emit_L
		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelGas.bhvr
		
		Part5	POWERS\EnergyBlast\EnergyCoreLocal.part
		Part1	POWERS\EnergyBlast\CoreEnergy.part
		Part2	POWERS\EnergyBlast\CoreEnergyBeam.part
		Part3	POWERS\EnergyBlast\CometBubbles2.part
		Part4	POWERS\EnergyBlast\CometBubbles3.part
		Lifespan 3
		#Magnet	Target
		#LookAt	Target
	End
	
	Event
		EName	ShotSound
		Type	Local
		At	Emit_L

		Sound PoliceBotBlast3 60.0 60.0 .4
	End
End

######################################################################################################
######################################################################################################
######################################################################################################
#####################################  Prime Projectile Right  #######################################
######################################################################################################
######################################################################################################
######################################################################################################

Condition
	On	Time
	Time	28

	Event
		EName 	HandGlowFlashL
		Type	local
		At	Emit_R
		bhvrOverride
			behavior
			Scale		.5 .5 .5
		End

		Part1	POWERS\EnergyBlast\HandGlowWhite3.part
		Part2	POWERS\EnergyBlast\HandGlowEmber3.part
		Lifespan 3
		
		PMagnet	WepL
	End
End

Condition
	On	Time
	Time	33

	Event
		EName	Prime1
		Type	start
		At	Emit_R
		
		Part2	POWERS\EnergyBlast\CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Sound WolfPackEnergyBlastHit 100.0 100.0 .7
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts02
		Type	Local
		At	Prime1
		
		Part1	POWERS\EnergyBlast\CometBubbles.part
		Part2	POWERS\EnergyBlast\CometBubblesBright.part
		Part3	POWERS\EnergyBlast\CometBubblesLocal.part
		Part4	POWERS\EnergyBlast\CometHead.part
		Part5	POWERS\EnergyBlast\EnergyCoreLocal.part
	End

End

######################################################################################################
###########################################  Gas Glow R  #############################################
######################################################################################################

Condition
	On	Time
	Time	30

	Event
		EName	cometPartsLocal
		Type	Local
		At	Emit_R
		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelGas.bhvr
		
		Part5	POWERS\EnergyBlast\EnergyCoreLocal.part
		Part1	POWERS\EnergyBlast\CoreEnergy.part
		Part2	POWERS\EnergyBlast\CoreEnergyBeam.part
		Part3	POWERS\EnergyBlast\CometBubbles2.part
		Part4	POWERS\EnergyBlast\CometBubbles3.part
		Lifespan 3
		#Magnet	Target
		#LookAt	Target
	End
	
	Event
		EName	ShotSound
		Type	Local
		At	Emit_L

		Sound PoliceBotBlast3 60.0 60.0 .4
	End
End

######################################################################################################
###########################################   K I L L S   ############################################
######################################################################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	cometParts
		Type	Destroy
	End
End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
	End

	Event
		EName 	cometParts02
		Type	Destroy
	End
End



End			