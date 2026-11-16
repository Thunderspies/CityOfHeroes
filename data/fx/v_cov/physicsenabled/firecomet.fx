#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	1500

Condition

	On 	Time
	Time 	10

	Event
		Type Local
		At origin
		Sound Firerain 100.0 100.0 .33
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		bhvr	:FireComet.bhvr
		Part1	\POWERS\FireControl\FireHands2.part
		Part2	\POWERS\FireControl\FireBoltTail2.part
		Part3	\POWERS\FireControl\FireBoltTail.part
		Part4	\POWERS\FireControl\SparkTrail.part
		Part5	\POWERS\FireControl\FireHands2.part

		CEvent 	:FireCometBurst.fx
		CThresh 	1
		CDestroy 	1
	End


End

Condition
	On	PrimeHit
		
		
	
	Event
		Ename	Prime
		Type	Destroy
	End

End


End	


			
