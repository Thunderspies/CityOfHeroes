#############################################################
## Costume Anim Entity - Shield - Roman 1
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ShieldOffset
		Type	Local
		At	LArmL
		Bhvr	:Female_Shield_Gem1.bhvr
		Anim	CustomShield_Gem1_01
	End
End

Condition
        On              Triggerbits
        Triggerbits     SHIELD
        DoMany          1

	Event
		EName	Sparkles
		Type	local
		At	LArmL
		Bhvr	:Female_Shield_Gem1.bhvr
                While   SHIELD
                Flags   OneAtATime
		Part	:Shield_Sparkle01.part
	End
End

#############################################################

End