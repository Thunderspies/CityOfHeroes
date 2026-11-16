#########################################################
## Continuing FX - Buff To-Hit (Alt)
#########################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#########################################################

Condition
	Event
		ENAME	x
		Type	Local
		At		Waist
		Geom	ChestPivotsBuffToHit
		BhvrOverride
				PositionOffset 0 0.1 0
				Spin 0 0.03 0
		End
	End

	Event
		Type	Local
		At		x
		AltPiv	2
		BhvrOverride
				PyrRotate 0 -0.8 0
		End
		part	:BuffToHitCrosshair02Alt.part
		part	:BuffToHitCrosshair03Alt.part
	End

	Event
		Type	Local
		At		x
		AltPiv	3
		BhvrOverride
				PyrRotate 0 -2.37 0
		End
		part	:BuffToHitCrosshair02Alt.part
		part	:BuffToHitCrosshair03Alt.part
	End

	Event
		Type	Local
		At		x
		AltPiv	5
		BhvrOverride
				PyrRotate 0 0.77159 0
		End
		part	:BuffToHitCrosshair02Alt.part
		part	:BuffToHitCrosshair03Alt.part
	End

	Event
		Type	Local
		At		x
		AltPiv	6
		BhvrOverride
				PyrRotate 0 2.34159 0
		End
		part	:BuffToHitCrosshair02Alt.part
		part	:BuffToHitCrosshair03Alt.part
	End
End


End
