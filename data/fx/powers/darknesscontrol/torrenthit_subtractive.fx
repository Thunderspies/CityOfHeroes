#############################################################
## TorrentHit_subtractive.fx
#############################################################

FxInfo
LifeSpan	20

#############################################################

Input
	InpName	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0



	Event
		ENAME   Tendrills
		Type	Local
		At	Hips
		Part1	:TorentHitTendril2.part
		Part1	:TorentHitTendril2_Subtractive.part
		LifeSpan	12
	End


	Event
		ENAME   Tendrills
		Type	Local
		At	Hips

		Part2	:TorentHitPuddle2.part
		Part2	:TorentHitPuddle2_Subtractive.part
	End

End

End


