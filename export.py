import bpy

context = bpy.context
obj = context.object
action =  context.object.animation_data.action
#print("fcurve_dic = {")
for fcurve in action.fcurves:

    print("Keyframe %s_%d[] = {" % (fcurve.data_path, fcurve.array_index))
    for kfp in fcurve.keyframe_points:
        #print("\t{", end="")
        vals = []
        for prop in ["co", "handle_left", "handle_right"]:
            co = getattr(kfp, prop)
            vals.append("{%.4f, %.4f}" %  co[:])
        print("\tKeyframe(%s)," % ", ".join(vals))
    print("};")

