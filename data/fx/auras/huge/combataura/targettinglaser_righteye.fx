#############################################################
## TargettingLaser1.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## RIGHT EYE ###########################################################

Condition
        On	Time
	Time	0

	Event
		EName	TargettingLaser_RightEye_Offset
		Type	Local
		At	Eyes
		BhvrOverride
			PositionOffset	-0.095 0.875 3.5
		End
	End
End

Condition
        On              Triggerbits
        Triggerbits     COMBAT
        DoMany          1

	Event
		EName	TargettingLaser_RightEye
		Type	Local
		At	Eyes
		BhvrOverride
			PositionOffset	-0.095 -0.0875 0.19
		End
                While   COMBAT
                Flags   OneAtATime
		Part	Auras\Male\CombatAura\Laser_Flare_Glow1.Part
		Part	Auras\Male\CombatAura\Laser_Flare_Core1.Part
		Part	Auras\Male\CombatAura\Laser_Beam_Core1.Part
		Part	Auras\Male\CombatAura\Laser_Beam_Glow1.Part
		POther	TargettingLaser_RightEye_Offset
	End
End

#############################################################

End