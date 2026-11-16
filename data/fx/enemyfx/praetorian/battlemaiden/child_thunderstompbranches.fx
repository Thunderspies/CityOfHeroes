#############################################################
## Thunder_Stomp_Branches.fx
#############################################################

FxInfo

Lifespan 450

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Ename	B1
		Type	startpositonly
		At	origin
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch_Root.bhvr
		lifespan 450
	End

	Event
		Ename	B2
		Type	local
		At	B1
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 450
	End

	Event
		Ename	B3
		Type	local
		At	B2
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 450
	End

	Event
		Ename	B4
		Type	local
		At	B3
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 450
	End

	Event
		Ename	B5
		Type	local
		At	B4
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 450
	End
End

################Static locs######################

Condition
	On 	time
	Time 	3

	Event
		Ename	X1
		Type	start
		At	b1
		lifespan 450
	End

	Event
		Ename	X2
		Type	start
		At	B2
		lifespan 450
		pother  X1
	End

	Event
		Ename	X3
		Type	start
		At	B3
		lifespan 450
		pother  X2
	End

	Event
		Ename	X4
		Type	start
		At	B4
		lifespan 450
		pother  X3
	End

	Event
		Ename	X5
		Type	start
		At	B5
		lifespan 450
		pother  X4
	End
end

################Particle cascade######################

Condition
	On 	time
	Time 	3

	Event
		Type	start
		At	X2
		lifespan 400
		part	:Thunder_Branch.part
		Part	:GeyserBranches_Streaks.part
		pother  X1
	End
End

Condition
	On 	time
	Time 	5

	Event
		Type	start
		At	X3
		lifespan 400
		part	:Thunder_Branch.part
		Part	:GeyserBranches_Streaks.part
		pother  X2
	End
End

Condition
	On 	time
	Time 	7

	Event
		Type	start
		At	X4
		lifespan 400
		part	:Thunder_Branch.part
		Part	:GeyserBranches_Streaks.part
		pother  X3
	End
End

Condition
	On 	time
	Time 	9

	Event
		Type	start
		At	X5
		lifespan 400
		part	:Thunder_Branch.part
		Part	:GeyserBranches_Streaks.part
		pother  X4
	End
End

#############################################################

End