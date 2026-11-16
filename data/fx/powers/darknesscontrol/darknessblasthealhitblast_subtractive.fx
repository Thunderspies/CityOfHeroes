#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300


#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	chest
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest

		Part2	:Blast_Subtractive.part

		Part3	:DarkEmber.part
		Part4	:DarkEmberLarge.part				
	End

	Event
		EName 	Prime2
		Type	Start 
		At	chest
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest

		Part5	:Blast_Subtractive.part
		Part5	:Blast.part

		Part3	:DarkBlastTendrill01.part
		Part4	:DarkBlastTendrill03.part
				
	End

	Event
		EName 	Prime3
		Type	Start 
		At	chest
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest
		
		Part1	:Blast_Subtractive.part

		Part2	:DarkBlastTendrill02.part
		Part3	:DarkBlastTendrill04.part
						
	End
	
	Event
		EName 	HitFake
		Type	local 
		At	chest
		Part1	:Blast_Subtractive.part
		Part2	:Blast_Subtractive.part
		lifespan 10				
	End
End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End
End


Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End


Condition
	On 	Prime3Hit
	Event
		EName 	Prime3
		Type	Destroy
	End

End

End	


			