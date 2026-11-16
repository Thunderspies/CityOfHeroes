#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	Inpname	root
End

Input  
	InpName	chest
End    

#########################################################

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	root
		Sound SoulDrain5_loop 80 80 .5
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End



Condition
	On 	Time
	Time 	25
	
	Event

		Type	Local
		At 	Chest
		Sound SoulDrain4 80 80 .72
	End

End


Condition
	On 	Time
	Time 	30

	

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part3	POWERS/DarknessControl/DamageHitMist.part
		Part1	POWERS/DarknessControl/SoulDrainHitTendril2.part
		Part2	POWERS/DarknessControl/SoulDrainHitPuddle2.part
		#Sound	SoulDrain5_loop 80 33 .5
	
	End

End

End
########################################################

			