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

lifespan	120

Condition
	On 	Time
	Time 	29

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Eviscerate5 100.0 100.0 .8
	End
End



Condition
	On 	Time
	Time 	70

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part	:EviscerateStreak.part
	End

	Event
		EName	Streak9
		Type	start
		At	wepL
		Part	:ClawEmbersBroadEvicerate.part
		#Part	:ClawEmbersEvicerate.part
		LifeSpan	3
	End

	Event
		EName	Streak8
		Type	start
		At	wepR
		
		Part	:ClawEmbersBroadEvicerate.part
		#Part	:ClawEmbersEvicerate.part
		LifeSpan	3
	End
End


Condition
	On 	Time
	Time 	72

	Event
		EName	streak
		Type	Destroy
		
	End
	

End

##########################################################
###2 hit
################
#Condition
#	On Time 
#	Time 70
#	
#	Event
#		EName	Pow
#		Type 	Local
#		At	T_Chest
#		Part1	:Clawflash.part
#		Part3	:ClawEmbers.part
#		Part4	:ClawEmbersBroad.part
#		Part5	:ClawsStar.part
#		Sound	Clawhit7 50.0 30.0 .58
#	End
#
#End
#
#Condition
#	On Time 
#	Time 72
#	
#	Event
#		EName	Pow
#		Type 	Destroy
#			
#	End
#
#End

End
##################################
