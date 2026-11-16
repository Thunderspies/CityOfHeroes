#########################################################
##	
##

FxInfo
LifeSpan	150
Flags		InheritAnimScale


Input  
	InpName	Target
End

Input  
	InpName	Mystic
End


###############################################################################
##################################  Shot 1  ###################################
###############################################################################
#
# this effect is made up of 8 shots, 2 from each of the 8 crab backpack arms 
# for each shot there are 3 quick flashing events or muzzel flares, a streaking event,
# and resulting smoke event.  
#
#

Condition
	On 	Time
	Time 	29
	

#	Event						# older node that now is a sound reference
#		ENAME   p1
#		Type	Start 
#		At	C_Fore_ToeL
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1				# muzzle flash03 - see other two below
		Type	Start
		At	C_Fore_ToeL

		Bhvr	:Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1				# streaking beam after muzzle flash
		Type	Start
		At	Blast1
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	27

	Event
		EName 	Blast2				# muzzle flash01
		Type	Start
		At	C_Fore_ToeL
		
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	28

	Event
		EName 	Blast3				# muzzle flash02
		Type	Start
		At	C_Fore_ToeL

		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Smoke				# smoke from barrel after shot
		Type	Local
		At	C_Fore_ToeL
		
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End

###############################################################################
##################################  Shot 2  ###################################
###############################################################################

Condition
	On 	Time
	Time 	37
	

#	Event
#		ENAME   p1_shot2
#		Type	Start 
#		At	C_Fore_ToeR
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1_shot2
		Type	Start
		At	C_Fore_ToeR

		Bhvr	:Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot2
		Type	Start
		At	Blast1_shot2
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	35

	Event
		EName 	Blast2_shot2
		Type	Start
		At	C_Fore_ToeR
		
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	36

	Event
		EName 	Blast3_shot2
		Type	Start
		At	C_Fore_ToeR

		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	38

	Event
		EName 	Smoke_shot2
		Type	Local
		At	C_Fore_ToeR
		
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End



###############################################################################
##################################  Shot 3  ###################################
###############################################################################

Condition
	On 	Time
	Time 	45
	

#	Event
#		ENAME   p1_shot3
#		Type	Start 
#		At	BendMystic
#		Bhvr	:Crab_ArmOffset_LeftArm.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1_shot3
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashLeft.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot3
		Type	Start
		At	Blast1_shot3
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	43

	Event
		EName 	Blast2_shot3
		Type	Start
		At	BendMystic
		
		Bhvr	:Arachnos_GunFlashLeft.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	44

	Event
		EName 	Blast3_shot3
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashLeft.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	46

	Event
		EName 	Smoke_shot3
		Type	Local
		At	BendMystic
		Bhvr	:Arachnos_GunFlashLeft.bhvr
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End





###############################################################################
##################################  Shot 4  ###################################
###############################################################################


Condition
	On 	Time
	Time 	54
	

#	Event
#		ENAME   p1_shot4
#		Type	Start 
#		At	BendMystic
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1_shot4
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashRight.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot4
		Type	Start
		At	Blast1_shot4
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	52

	Event
		EName 	Blast2_shot4
		Type	Start
		At	BendMystic
		
		Bhvr	:Arachnos_GunFlashRight.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	53

	Event
		EName 	Blast3_shot4
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashRight.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	55

	Event
		EName 	Smoke_shot4
		Type	Local
		At	BendMystic
		Bhvr	:Crab_ArmOffset_RightArm.bhvr
		
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End




###############################################################################
##################################  Shot 5  ###################################
###############################################################################

Condition
	On 	Time
	Time 	63
	

#	Event
#		ENAME   p1_shot5
#		Type	Start 
#		At	C_Fore_ToeL
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1_shot5
		Type	Start
		At	C_Fore_ToeL

		Bhvr	:Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot5
		Type	Start
		At	Blast1_shot5
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	61

	Event
		EName 	Blast2_shot5
		Type	Start
		At	C_Fore_ToeL
		
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	62

	Event
		EName 	Blast3_shot5
		Type	Start
		At	C_Fore_ToeL

		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	64

	Event
		EName 	Smoke_shot5
		Type	Local
		At	C_Fore_ToeL
		
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End




###############################################################################
##################################  Shot 6  ###################################
###############################################################################

Condition
	On 	Time
	Time 	73
	

#	Event
#		ENAME   p1_shot6
#		Type	Start 
#		At	C_Fore_ToeR
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1_shot6
		Type	Start
		At	C_Fore_ToeR

		Bhvr	:Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot6
		Type	Start
		At	Blast1_shot6
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	71

	Event
		EName 	Blast2_shot6
		Type	Start
		At	C_Fore_ToeR
		
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	72

	Event
		EName 	Blast3_shot6
		Type	Start
		At	C_Fore_ToeR

		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	74

	Event
		EName 	Smoke_shot6
		Type	Local
		At	C_Fore_ToeR
		
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End




###############################################################################
##################################  Shot 7  ###################################
###############################################################################

Condition
	On 	Time
	Time 	80
	

#	Event
#		ENAME   p1_shot7
#		Type	Start 
#		At	BendMystic
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End

	Event
		EName 	Blast1_shot7
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashLeft.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot7
		Type	Start
		At	Blast1_shot7
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	78

	Event
		EName 	Blast2_shot7
		Type	Start
		At	BendMystic
		
		Bhvr	:Arachnos_GunFlashLeft.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	79

	Event
		EName 	Blast3_shot7
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashLeft.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	81

	Event
		EName 	Smoke_shot7
		Type	Local
		At	BendMystic
		Bhvr	:Arachnos_GunFlashLeft.bhvr
		
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End





###############################################################################
##################################  Shot 8  ###################################
###############################################################################


Condition
	On 	Time
	Time 	89
	

#	Event
#		ENAME   p1_shot8
#		Type	Start 
#		At	BendMystic
#		#Bhvr	:Arachnos_GunFlash.bhvr
#		#Geom	FlameThrower
#		#Geom	TESTING_Target
#		#Sound	Assault3 120.0 30.0 1.0
#	End
#
	Event
		EName 	Blast1_shot8
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashRight.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End



	Event
		EName 	Flare1_shot8
		Type	Start
		At	Blast1_shot8
		#Altpiv 1
		
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
		LifeSpan  5
	End

End

##################################################

Condition
	On 	Time
	Time 	87

	Event
		EName 	Blast2_shot8
		Type	Start
		At	BendMystic
		
		Bhvr	:Arachnos_GunFlashRight.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End

#################################################

Condition
	On 	Time
	Time 	88

	Event
		EName 	Blast3_shot8
		Type	Start
		At	BendMystic

		Bhvr	:Arachnos_GunFlashRight.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
		lifespan  4
	End

End


################################################

Condition
	On 	Time
	Time 	90

	Event
		EName 	Smoke_shot8
		Type	Local
		At	BendMystic
		Bhvr	:Crab_ArmOffset_RightArm.bhvr
		
		Part	:Crab_GunSmoke01.part
		lifespan  50
	End

End





End				