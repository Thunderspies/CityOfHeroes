#############################################################
## TargettingLaser1.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## LEFT EYE ###########################################################

Condition
        On	Time
	Time	0

	Event
		EName	TargettingLaser_LeftEye_Offset
		Type	Local
		At	Eyes
		BhvrOverride
			PositionOffset	0.095 0.875 3.5
		End
	End
End

Condition
        On              Triggerbits
        Triggerbits     COMBAT
        DoMany          1

	Event
		EName	TargettingLaser_LeftEye
		Type	Local
		At	Eyes
		BhvrOverride
			PositionOffset	0.095 -0.04 0.25
		End
                While   COMBAT
                Flags   OneAtATime
		Part	Auras\Male\CombatAura\Laser_Flare_Glow1.Part
		Part	Auras\Male\CombatAura\Laser_Flare_Core1.Part
		Part	Auras\Male\CombatAura\Laser_Beam_Core1.Part
		Part	Auras\Male\CombatAura\Laser_Beam_Glow1.Part
		POther	TargettingLaser_LeftEye_Offset
	End
End

#############################################################

End