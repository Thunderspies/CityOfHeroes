#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	WepR
End

Input  
	Inpname	T_Root
End

LifeSpan	200

########################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	WepR
		Part1	:WebControl01a.part
		Part2	:WebControl02a.part
		Part3	:WebInnerGlowa.part
		Part5	:WebBallTail.part
		Bhvr	Behaviors/NetProjectile.bhvr
				
		Magnet	T_Root
		Lookat	T_Root

	End

	Event
		EName 	Prime2
		Type	start
		At	WepR
		Part1	:WebOuterGlowa.part
		Part2	:WebBallTailGlow.part
		#Part3	:WebRinga.part
		#Part4	:WebArchInverta.part
		Part5	:WebBallTail2.part
		Bhvr	Behaviors/NetProjectile.bhvr
		
		Magnet	T_Root
		Lookat	T_Root

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


End