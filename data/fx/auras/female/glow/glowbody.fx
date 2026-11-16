#############################################################
## GlowBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	Auras\Male\Glow\HeadOffset1.bhvr
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	Head
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	HandL
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	Chest
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet UarmR
		POther  UarmR
	End

	Event
		Type	Local
		At	UarmR
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	HandR
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UlegL
		part	Auras\Male\Glow\UlegGlow01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		Type	Local
		At	UlegR
		part	Auras\Male\Glow\UlegGlow01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	Auras\Male\Glow\BodyGlow01.part
		Pmagnet FootL
	End

End

#############################################################

End