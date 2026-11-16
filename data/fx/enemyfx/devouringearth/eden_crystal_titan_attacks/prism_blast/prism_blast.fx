#########################################################
##	Prism Blast
##
FxInfo

LifeSpan	200


Condition	
	On	Time
	Time	0

	Event	
		Type	Local
		At	WepR
		Sound titanprismblast 90 90 .8
	End
End




Condition
	On 	Time
	Time 	18

	Event
		EName	Prime2
		Type	Local
		At	WepR
		Part	:Hand_Glow.part
		Lifespan	10
	End

	Event
		EName	Prime2
		Type	Posit
		At	WepR
		Magnet	Target
		LookAt	Target
		Bhvr	:Cone_Projectile.bhvr
		PMagnet	WepR		
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
	On 	Time
	Time 	18

	Event
		EName	Prime
		Type	Start
		At	WepR
		Magnet	Target
		LookAt	Target
		Part	:Prism_Beam_01.part
		Bhvr	:Cone_Projectile.bhvr
		PMagnet	WepR
		Sound RedToss 90 90 .8
		
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End			