#########################################################
##	Fireweapon
########################################################
FxInfo

Flags    InheritAlpha

lifespan	550

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Fast6 80.0 80.0 .8
	End
End


#################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		
		BhvrOverride
			FadeInLength		20
			FadeOutLength		5

			Scale			.1 .1 .1
			ScaleRate		1.05 1.05 1.05
			 EndScale		.8 1 .8
		End
		Geom	Mantis_Silver_Blade_Left
		LifeSpan 	62
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		
		BhvrOverride
			FadeInLength		20
			FadeOutLength		5

			Scale			.1 .1 .1
			ScaleRate		1.05 1.05 1.05
			 EndScale		.8 1 .8
		End
		Geom	Mantis_Silver_Blade_Right
		LifeSpan 	62
			
	End
End

###############################################################################
##SpikeShrink
###################################

Condition
	On 	Time
	Time 	63

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleDown.bhvr
		
		BhvrOverride
			FadeInLength		5
			FadeOutLength		40

			Scale			.8 1 .8
			ScaleRate		1.05 1.05 1.05
			 EndScale		.1 .1 .1
		End
		Geom	Mantis_Silver_Blade_Left
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleDown.bhvr
		
		BhvrOverride
			FadeInLength		5
			FadeOutLength		40

			Scale			.8 1 .8
			ScaleRate		1.05 1.05 1.05
			 EndScale		.1 .1 .1
		End
		Geom	Mantis_Silver_Blade_Right
			
	End
End

##############################################################################


Condition
	On 	Time
	Time 	49

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part1	Powers/Claws/EviscerateStreak.part
		lifespan	250
	End

End



Condition
	On 	Time
	Time 	40
	Event
		EName	Streak9
		Type	start
		At	wepL
		#Part1	:QuillsBroadEvicerate.part
		
		LifeSpan	3
	End

	Event
		EName	Streak8
		Type	start
		At	wepR
		
		#Part1	:QuillsBroadEvicerate.part
		
		LifeSpan	3
	End
End


Condition
	On 	Time
	Time 	47

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
#	Time 34
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
#		Sound	thornpus 50 30 .7
#		lifespan	250
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
#		sound	quills6 80 70 .88
#		lifespan	250
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
#		lifespan	250
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
#		LifeSpan	51
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
#		lifespan	250
#	End
#	
#End
#
#Condition
#	On 	Time
#	Time 	41
#
#
#	Event
#
#		EName	GasSpurts2
#		Type	Local
#		At	T_hips
#				
#		Part1	:PoisonSpark2.part
#		LifeSpan	58
#	End
#
#End
#

End
##################################
