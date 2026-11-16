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
		EName 	Hand1
		Type	Local 
		At	T_UarmR
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
		Sound miasma3 100 100 .8
		
		PMagnet	T_LarmR
	End

	Event
		EName 	Hand3
		Type	Local
		At	T_UarmR
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	T_LarmL
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local 
		At	T_LarmR
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	T_WepR
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	T_LarmL
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	T_WepL
	End

End


Condition
	On 	Time
	Time 	74


	Event
		EName 	Prime
		Type	Start 
		At	chest
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest

		Part2	:Blast.part

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
		
		Part1	:Blast.part

		Part2	:DarkBlastTendrill02.part
		Part3	:DarkBlastTendrill04.part
						
	End


End


Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	chest
		Part1	:Blast.part
		Part2	:Blast.part
		lifespan	10				
	End
	
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

Condition
	On 	Time
	Time	93

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

	Event
		EName 	Hand3
		Type	Destroy

	End

	Event
		EName 	Hand4
		Type	Destroy

	End

End

Condition
	On 	Time
	Time	95


	Event
		EName 	Hand5
		Type	Destroy

	End

	Event
		EName 	Hand6
		Type	Destroy

	End

End

End	


			