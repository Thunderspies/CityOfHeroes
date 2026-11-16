#############################################################
## DarknessBlastHit.fx
#############################################################

FxInfo
LifeSpan	20

########################################################

Input
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	0



	Event
		ENAME   Tendrills
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\DarkBlast\DarkBlastHitTendril2.part
		Part2	CustomizeablePowers\DarkBlast\DarkBlastHitPuddle2.part

	End

End

#########################################################

End


