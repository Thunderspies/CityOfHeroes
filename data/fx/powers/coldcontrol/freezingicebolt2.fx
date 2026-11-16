#############################################################
## FreezingIceBolt2.fx
#############################################################

FxInfo

LifeSpan 21

#############################################################
Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Bhvr	behaviors\IceRain2.bhvr
		Geom	FX_IceBlast
		Part1	POWERS\ColdControl\icebolttail3a.part
		Part2	POWERS\ColdControl\icebolttail2a.part
		Part3	POWERS\ColdControl\IceBoltSnowtrail2.part
		Part4	POWERS\ColdControl\IceBoltMistTrail2.part
	End
End

#############################################################

End