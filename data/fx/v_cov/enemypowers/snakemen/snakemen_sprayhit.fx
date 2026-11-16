#########################################################
##	
##

FxInfo

LifeSpan	200

###############################################################################
############################# Cloud Child FX  #################################
###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Cloud
		Type	Start 
		At	Origin

#		ChildFx :SM_CloudAOE.fx
	End

End

###############################################################################
########################  Contaminate ground up  ##############################
###############################################################################

Condition
	On 	Time
	Time    2


	Event

		EName	GasRoot
		Type	Start
		At	Origin
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End
End



##########  Toes  ###########

Condition
	On 	Time
	Time    10


	Event

		EName	GasToeR
		Type	Local
		At	ToeR
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

	Event

		EName	GasToeL
		Type	Local
		At	ToeL
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

End

##########  Ankles  ###########

Condition
	On 	Time
	Time    12


	Event

		EName	GasAnkR
		Type	Local
		At	FootR
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

	Event

		EName	GasAnkL
		Type	Local
		At	FootL
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

End


##########  Knees  ###########

Condition
	On 	Time
	Time    14


	Event

		EName	GasLLegR
		Type	Local
		At	LLegR
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

	Event

		EName	GasLLegL
		Type	Local
		At	LLegL
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

End


##########  ULeg  ###########

Condition
	On 	Time
	Time    16


	Event

		EName	GasULegR
		Type	Local
		At	ULegR
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

	Event

		EName	GasULegL
		Type	Local
		At	ULegL
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

End


##########  Hips  ###########

Condition
	On 	Time
	Time    18


	Event

		EName	GasHips
		Type	Local
		At	Hips
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End
End


##########  Waist  ###########

Condition
	On 	Time
	Time    20


	Event

		EName	GasWaist
		Type	Local
		At	Waist
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End
End


##########  Chest  ###########

Condition
	On 	Time
	Time    22


	Event

		EName	GasChest
		Type	Local
		At	Chest
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End
End

##########  Col  ###########

#Condition
#	On 	Time
#	Time    16
#
#
#	Event
#
#		EName	GasColR
#		Type	Local
#		At	Col_R
#		
#		Part1	:SM_SpitGasHitUp.part
#		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
#		Lifespan 45
#	End
#
#	Event
#
#		EName	GasColL
#		Type	Local
#		At	Col_L
#		
#		Part1	:SM_SpitGasHitUp.part
#		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
#		Lifespan 45
#	End
#
#End
#
#
##########  UArm  ###########

Condition
	On 	Time
	Time    24


	Event

		EName	GasUArmR
		Type	Local
		At	UArmR
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

	Event

		EName	GasUArmL
		Type	Local
		At	UArmL
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End

End


##########  LArm  ###########
#
#Condition
#	On 	Time
#	Time    28
#
#
#	Event
#
#		EName	GasLArmR
#		Type	Local
#		At	LArmR
#		
#		Part1	:SM_SpitGasHitUp.part
#		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
#		Lifespan 45
#	End
#
#	Event
#
#		EName	GasLArmL
#		Type	Local
#		At	LArmL
#		
#		Part1	:SM_SpitGasHitUp.part
#		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
#		Lifespan 45
#	End
#
#End


##########  Hand  ###########
#
#Condition
#	On 	Time
#	Time    20
#
#
#	Event
#
#		EName	GasHandR
#		Type	Local
#		At	HandR
#		
#		Part1	:SM_SpitGasHitUp.part
#		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
#		Lifespan 45
#	End
#
#	Event
#
#		EName	GasHandL
#		Type	Local
#		At	HandL
#		
#		Part1	:SM_SpitGasHitUp.part
#		Part2	:SM_SpitGasSpecksHitUp.part
##		Part3	:SM_SpitGasBubblesUp.part
#		Lifespan 45
#	End
#
#End
#
#
##########  head  ###########

Condition
	On 	Time
	Time    26


	Event

		EName	GasHead
		Type	Local
		At	Head
		
		Part1	:SM_SpitGasHitUp.part
		Part2	:SM_SpitGasSpecksHitUp.part
#		Part3	:SM_SpitGasBubblesUp.part
		Lifespan 45
	End
End


###############################################################################
###################################  Smoke  ###################################
###############################################################################

Condition
	On 	Time
	Time    2


	Event

		EName	Smoke
		Type	Local
		At	Origin
		
		ChildFX	:SM_AOESpraySmoke.fx			
	End
End


End				