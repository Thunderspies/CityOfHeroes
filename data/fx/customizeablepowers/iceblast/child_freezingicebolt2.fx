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
		BhvrOverride
			TintGeom		1
		End
		Geom	tintable_FX_IceBlast
		Part2	:icebolttail2a.part
		Part1	:icebolttail3a.part
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail2.part
	End
End

#############################################################

End