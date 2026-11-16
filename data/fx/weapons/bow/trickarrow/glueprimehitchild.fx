#########################################################
## Glue Arrow - Prime Hit Child
#########################################################

FxInfo

#########################################################

Condition

	Event	
		ENAME	GlueArrowSplatBase1
		Type	start
		At	origin
		Bhvr	:GlueSplatInitial.bhvr
		Splat	Water.tga
		LifeSpan 900
	End

	Event	
		ENAME	GlueArrowSplatBase2
		Type	start
		At	origin
		Bhvr	:GlueSplatInitial.bhvr
		BhvrOverride
			Alpha	128
			Scale	48 8 48
		End
		Splat	Speckled.tga
		LifeSpan	900	
	End
End

#########################################################

Condition
	On	Time
	Time	1
	Repeat	8

	Event	
		ENAME	GlueSploches1
		Type	start
		At	origin
		Bhvr	:glueSplat.bhvr
		Splat	glue.tga
		LifeSpan	900	
	End
End

#########################################################

End		