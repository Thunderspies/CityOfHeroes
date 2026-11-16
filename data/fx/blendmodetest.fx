#############################################################
## BlendModeTest.fx
#############################################################

FXInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	AlphaBlend
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		-4 2 0
		End
		Part	ParticleTest_BlendMode0.part
	End

	Event
		EName	AlphaBlend
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		-2 2 0
		End
		Part	ParticleTest_BlendMode1.part
	End

	Event
		EName	AlphaBlend
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 2 0
		End
		Part	ParticleTest_BlendMode2.part
	End

	Event
		EName	AlphaBlend
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		2 2 0
		End
		Part	ParticleTest_BlendMode3.part
	End

	Event
		EName	AlphaBlend
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		4 2 0
		End
		Part	ParticleTest_BlendMode4.part
	End
End

#############################################################

End
