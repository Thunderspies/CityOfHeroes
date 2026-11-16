#############################################################
## FireComet.fx
#############################################################

FxInfo

LifeSpan 1500

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	origin
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
		Part1	:FireHands2.part
		Part2	:FireBoltTail2.part
		Part3	:FireBoltTail.part
		Part4	:SparkTrail.part
		Part5	:FireHands2.part
		CEvent 	:FireCometBurst.fx
		CThresh 1
		CDestroy 1
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End