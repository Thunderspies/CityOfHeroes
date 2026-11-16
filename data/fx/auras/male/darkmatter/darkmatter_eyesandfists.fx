#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Eyes
			Type Local
			EName EyeAnchor
			Geom CircleOfThorns
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Matter_Eye_L.part
			Part :Eye_Glow.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Matter_Eye_R.part
			Part :Eye_Glow.part
		End

		Event
			At WepL
			Type Local
			EName LeftArm
			PMagnet UArmL
			POther UArmL
			Part Auras\Male\DarkMatter\Matter_Fists.part
			Part Auras\Male\DarkMatter\Matter_Fists_Move.part
		End

		Event
			At WepR
			Type Local
			EName RightArm
			PMagnet UArmR
			POther UArmR
			Part Auras\Male\DarkMatter\Matter_Fists.part
			Part Auras\Male\DarkMatter\Matter_Fists_Move.part
		End

	End

End
