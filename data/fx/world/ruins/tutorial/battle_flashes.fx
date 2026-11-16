#############################################################
## Smoke_StormCloud.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Origin
End

#############################################################
Condition
	On	TIme
	Time	0

	Event
		EName	TempAnchor
		Type	Local
		At	Origin
	End

	Event
		Type	Start
		At	TempAnchor
		BhvrOverride
			PositionOffset	-0 -3 -12
			PYRRotate	90 0 0
		End
		Part	:Flashes_Cloud2.part
	End
End

End