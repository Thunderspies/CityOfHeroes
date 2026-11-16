#############################################################
## IceShield.fx
#############################################################

FxInfo

#############################################################

Condition
        On              Triggerbits
        Triggerbits     SHIELD
        DoMany          1

	Event
		EName	TalsorianShield
		Type	Local
		At	Hips
                While   SHIELD
                Flags   OneAtATime
		Part1	:IceBoltMistB.part
		Part2	POWERS\ColdControl\ChillTouchSnowA.part
		Part1	POWERS\ColdControl\ColdStars.part
		Part2	:FrostBits.part
	End
End

#############################################################

End