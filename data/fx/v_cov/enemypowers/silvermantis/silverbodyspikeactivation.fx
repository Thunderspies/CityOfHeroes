#########################################################
##	
##

FxInfo

#LifeSpan	100



Flags    InheritAlpha

###########################################################
##  Spikes sound and particles
################################################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	HandL
		Sound Quills1b 70 70 .76
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest	
		Part	:silverSparks2.part
#		Part2	:MustardGasSpecksHitA.part
#		Part1	:MustardGasHitA.part
		
		LifeSpan	7	
	End

End

Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurtsb
		Type	Local
		At	HandR
				
		Part	:silverSparks2.part
#		Part1	:MustardGasHitBA.part
#		Part2	:MustardGasSpecksHitBA.part
		lifeSpan	16
	End

End	

Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurts
		Type	Local
		At	HandL
		
		Part	:silverSparks2.part
#		Part1	:MustardGasHitBA.part
#		Part2	:MustardGasSpecksHitBA.part
		lifeSpan	16
	End

End	


################################################################
##  Hand Spikes
################################################################


Condition
	On 	Time
	Time 	12



	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Mantis_Silver_Spikes_WepL
		Part4	:SilverGlow1.part
		Part	:silverSparks2.part
			
	End

	
End

Condition
	On 	Time
	Time 	12



	Event
		EName	Spike2
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Mantis_Silver_Spikes_WepR
		Part4	:SilverGlow1.part
		Part	:silverSparks2.part
			
	End

	
End


################################################################
##  Arm Spikes
################################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Spike02b
		Type	Local
		At	LarmL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Mantis_Silver_Spikes_LarmL
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	HandL
			
	End


	Event
		EName	Spike02b
		Type	Local
		At	LarmR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Mantis_Silver_Spikes_LarmR
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	HandR
			
	End

End
 
Condition
	On 	Time
	Time 	7

	Event
		EName	Spike02b
		Type	Local
		At	UarmR
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Mantis_Silver_Spikes_UarmR
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	LarmR
			
	End



	Event
		EName	Spike02b
		Type	Local
		At	UarmL
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Mantis_Silver_Spikes_UarmL
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	LarmL
			
	End

##############################################################


End
 
Condition
	On 	Time
	Time 	4

	Event
		EName	Spike02b
		Type	Local
		At	SPadL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Mantis_Silver_Spikes_SPadL
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
			
	End


	Event
		EName	Spike02b
		Type	Local
		At	SpadR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Mantis_Silver_Spikes_SpadR
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
			
	End

End

################################################################
##  Back  Spikes
################################################################


Condition
	On 	Time
	Time 	3
	Event
		EName	Spike02b
		Type	Local
		At	waist
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Mantis_Silver_Spikes_Waist
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	Hips
			
	End

End

Condition
	On 	Time
	Time 	1

	Event
		EName	Spike02b
		Type	Local
		At	chest
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Mantis_Silver_Spikes_Chest
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	Neck
		
		Part4	:SilverGlow1.part
		Pmagnet	Hips
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	waist
		
		Part4	:SilverGlow1.part
		Pmagnet	UarmL
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	waist
		
		Part4	:SilverGlow1.part
		Pmagnet	UarmR
			
	End

End

#######################################################
## Leg  Spikes
###############################################

Condition
	On 	Time
	Time 	6
	Event
		EName	Spike02b
		Type	Local
		At	UlegL
		
		Bhvr	Behaviors/PoisonSpikeScale2.bhvr
		Geom	Mantis_Silver_Spikes_UlegL
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	LlegL
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	UlegR
		
		Bhvr	Behaviors/PoisonSpikeScale2.bhvr
		Geom	Mantis_Silver_Spikes_UlegR
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	LlegR
			
	End
End


##############################################################


Condition
	On 	Time
	Time 	9
	Event
		EName	Spike02b
		Type	Local
		At	LlegR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Mantis_Silver_Spikes_LlegR
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	FootR
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Mantis_Silver_Spikes_LlegL
		Part	:silverSparks2.part
		Part4	:SilverGlow1.part
		Pmagnet	FootL

	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegL
		
		Part4	:SilverGlow1.part
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegR
		
		Part4	:SilverGlow1.part
			
	End

		
End

Condition
	On 	Time
	Time 	12
	Event
		EName	Spike02b
		Type	Local
		At	FootL
		
		Part4	:SilverGlow1.part
		Part	:silverSparks2.part
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	FootR
		
		Part4	:SilverGlow1.part
		Part	:silverSparks2.part
			
	End

End


End				