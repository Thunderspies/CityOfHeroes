#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Root
End

Input
	Inpname	WepR
End

Input
	Inpname	WepL
End

Input
	Inpname	T_Chest
End

Input
	Inpname	T_UarmL
End

Input
	Inpname	T_UarmR
End

Input
	Inpname	T_Hips
End

Input
	Inpname	T_Head
End

lifespan	120

Condition
	On 	Time
	Time 	29

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Fast6 80.0 80.0 .8
	End
End



Condition
	On 	Time
	Time 	73

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part1	Powers/Claws/EviscerateStreak.part
	End

End



Condition
	On 	Time
	Time 	6

	Event
		EName	Streak9
		Type	local
		At	HandL
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	PoisonSpikeLeft
		
		LifeSpan	63
	End

	Event
		EName	Streak8
		Type	local
		At	HandR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	PoisonSpikeRight
		
		LifeSpan	63
	End
End


Condition
	On 	Time
	Time 	83

	Event
		EName	streak
		Type	Destroy
		
	End
	

End

#########################################################
##2 hit
###############
#Condition
#	On Time 
#	Time 70
#	
#	Event
#
#		EName	Gas
#		Type	Local
#		At	T_chest
#				
#		Part1	:MustardGasHitA.part
#		Part2	:MustardGasSpecksHitA.part
#		Part3	:MustardGasBubblesA.part
#		Sound	poisonhit2 50 30 .7
#		
#	End
#
#	Event
#
#		EName	Gas1
#		Type	Local
#		At	T_UArmL
#				
#		#Part1	:MustardGasHitA.part
#		Part2	:MustardGasSpecksHitA.part
#		Part3	:MustardGasBubblesA.part
#		
#	End
#
#	Event
#
#		EName	Gas2
#		Type	Local
#		At	T_UArmR
#				
#		#Part1	:MustardGasHitA.part
#		Part2	:MustardGasSpecksHitA.part
#		Part3	:MustardGasBubblesA.part
#		
#	End
#
#
#	Event
#
#		EName	GasSpurts
#		Type	Local
#		At	T_chest
#				
#		Part1	:PoisonSpark2.part
#		LifeSpan	87
#	End
#
#	Event
#
#		EName	HeadConfusionClouds
#		Type	Local
#		At	T_Head
#				
#		#Part1	:MustardGasHeadA.part
#		Part2	:MustardGasSpecksHeadA.part
#		Part3	:MustardGasBubblesHeadA.part
#	End
#	
#End
#
#Condition
#	On 	Time
#	Time 	72
#
#
#	Event
#
#		EName	GasSpurts2
#		Type	Local
#		At	T_hips
#				
#		Part1	:PoisonSpark2.part
#		LifeSpan	94
#	End
#
#End
#
Condition
	On 	Time
	Time 	84

	Event

		EName	all
		Type	destroy
			
	End


End


End
####################################################################
