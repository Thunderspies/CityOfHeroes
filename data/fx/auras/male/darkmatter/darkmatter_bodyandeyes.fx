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
			At Head
			Type Local
			EName HeadParts
			PMagnet Chest
			POther Chest
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

		Event
			At Chest
			Type Local
			EName ChestParts
			PMagnet Hips
			POther Hips
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Big.part
			Part Auras\Male\DarkMatter\Matter_Clump.part
		End

		Event
			At Hips
			Type Local
			EName HipParts
			PMagnet Chest
			POther Chest
			Part Auras\Male\DarkMatter\Matter_Clump.part
		End

		Event
			At LArmL
			Type Local
			EName LeftArm
			PMagnet UArmL
			POther UArmL
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

		Event
			At LArmR
			Type Local
			EName RightArm
			PMagnet UArmR
			POther UArmR
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

		Event
			At ULegL
			Type Local
			EName LeftLegUp
			PMagnet LLegL
			POther LLegL
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

		Event
			At ULegR
			Type Local
			EName RightLegUp
			PMagnet LLegR
			POther LLegR
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

		Event
			At LLegL
			Type Local
			EName LeftLeg
			PMagnet ULegL
			POther ULegL
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

		Event
			At LLegR
			Type Local
			EName RightLeg
			PMagnet ULegR
			POther ULegR
			Part Auras\Male\DarkMatter\Matter_Small_Move.part
			Part Auras\Male\DarkMatter\Matter_Small.part
		End

	End

End
