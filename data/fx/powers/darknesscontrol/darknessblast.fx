#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

Input  
	InpName	Origin
End

Input  
	InpName	Target
End
   
Input  
	InpName	mystic
End

Input  
	Inpname	WepL
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	UarmR
End


Input  
	Inpname	WepR
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
		Sound miasma3 100 100 .8
		
		PMagnet	LarmR
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmL
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepL
	End

End


Condition
	On 	Time
	Time 	74


	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target

		Part2	:Blast.part

		Part3	:DarkEmber.part
		Part4	:DarkEmberLarge.part


						
	End

	Event
		EName 	Prime2
		Type	Start 
		At	WepR
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target

		Part5	:Blast.part
		
		Part3	:DarkBlastTendrill01.part
		Part4	:DarkBlastTendrill03.part
				
	End

	Event
		EName 	Prime3
		Type	Start 
		At	WepR
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		
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
		At	target
		Part1	:Blast.part
		Part2	:Blast.part
						
	End
	
	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End

	Event
		EName 	Prime3
		Type	Destroy

	End

End

Condition
	On 	Time
	Time	50

	Event
		EName 	HitFake
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


			