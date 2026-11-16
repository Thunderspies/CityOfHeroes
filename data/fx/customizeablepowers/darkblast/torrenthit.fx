#############################################################
## TorrentHit.fx
#############################################################

FxInfo
LifeSpan	20

########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0



	Event
		ENAME   Tendrills
		Type	Local
		At	Hips
		Bhvr	behaviors\GenericParticleFade.bhvr

		Part1	CustomizeablePowers\DarkBlast\TorentHitTendril2.part
		LifeSpan	12
	End


	Event
		ENAME   Tendrills
		Type	Local
		At	Hips
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\DarkBlast\TorentHitPuddle2.part

	End

End

#########################################################

End


