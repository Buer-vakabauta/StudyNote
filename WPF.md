PS:该笔记仅供查阅,经验分享使用.不作教学.包括一些实际使用过程中的问题,以及内容补充.
使用Visual Studio进行开发与调试

# WPF生命周期

---

### Application的生命周期

OnStartup(应用程序启动)->OnActivated(激活应用程序)->OnDeactivated(由激活变为非激活)->OnExit(程序退出)

### Window窗体的生命周期

![Window窗体的生命周期](https://github.com/Buer-vakabauta/StudyNote/tree/main/assets/Wpf1.png)

### 总体的生命周期

Application的OnStartup，然后是主窗体的SourceInitialized，然后依次执行了Application的OnActivated和MainWindow的Activated，最后直到主窗体Closed，才轮到Application的OnExit。

---

---





# 控件父类方法和属性

### 1.`DependencyObject`类

> `DependencyObject`：WPF 依赖属性系统的基类
>  它提供了存储、读取、清理依赖属性值的功能。
>  所有控件（Button、TextBox 等）最终都继承自它。

---
#### 构造函数
- `public DependencyObject();`  
  创建一个依赖对象的实例。通常不直接实例化 `DependencyObject`，而是在派生类（控件、可视元素等）中使用。

#### 属性

- `public DependencyObjectType DependencyObjectType { get; }`  
  返回该实例的 **依赖对象类型描述**（不同于普通的 `System.Type`，用于 WPF 依赖属性系统的类型元信息与快速查找/优化）。

- `public bool IsSealed { get; }`  
  指示此实例是否被**封印（只读）**。被封印后对象的某些结构性状态不可再更改（通常框架内部使用；大多数常见控件实例为 `false`）。

#### 公共方法

- `public void ClearValue(DependencyProperty dp);`  
  **清除本地值**：从该属性移除本地设置，让属性值回退到更低优先级的来源（如样式、继承、默认值、绑定/动画结果等）。

- `public void ClearValue(DependencyPropertyKey key);`  
  使用 **只读依赖属性的 Key** 清除其本地值。仅供**拥有者类型**或受信范围使用（外部无法拿到 `DependencyPropertyKey`）。

- `public void CoerceValue(DependencyProperty dp);`  
  触发该属性的 **强制回调（CoerceValueCallback）**，以当前上下文重新**钳制/修正**属性值（常用于根据其它属性状态限定取值范围）。

- `public sealed override bool Equals(object obj);`  
  引用相等的密封重写。判断与另一个对象是否相同实例。**已密封**，派生类不可再改写。

- `public sealed override int GetHashCode();`  
  与 `Equals` 一致性对应的哈希实现。**已密封**，派生类不可再改写。

- `public LocalValueEnumerator GetLocalValueEnumerator();`  
  获取一个枚举器，枚举此对象上**所有设置了本地值**的依赖属性（便于调试或批量检查）。

- `public object GetValue(DependencyProperty dp);`  
  按 WPF **属性优先级**（动画 > 本地值 > 样式触发器 > 样式 Setter > 模板 > 继承 > 默认值 …）**获取当前有效值**。  
  > 注意：这可能是动画/绑定评估后的值，而非仅本地值。

- `public void InvalidateProperty(DependencyProperty dp);`  
  将该属性标记为**无效**并请求重新求值（促使绑定、继承、强制回调等在下一次布局/渲染前刷新）。

- `public object ReadLocalValue(DependencyProperty dp);`  
  仅读取 **本地值**（不考虑样式/继承/动画等），未设置时返回 `DependencyProperty.UnsetValue`。

- `public void SetCurrentValue(DependencyProperty dp, object value);`  
  **不改变现有优先级来源**（例如不打断绑定/样式）的前提下，设置“当前值”。  
  典型用途：你想**临时赋值**，但仍希望绑定或样式在之后继续生效并能覆盖它。

- `public void SetValue(DependencyProperty dp, object value);`  
  设置 **本地值**。  
  > 提醒：若该属性**已绑定**，直接 `SetValue` 往往会**替换为本地值并移除绑定**（等价于打断绑定）；若要保留绑定，请考虑 `SetCurrentValue`。

- `public void SetValue(DependencyPropertyKey key, object value);`  
  使用 **只读依赖属性的 Key** 在拥有者类型内部设置其值（外部代码无法设置只读依赖属性）。

#### 受保护/内部扩展点

- `protected virtual void OnPropertyChanged(DependencyPropertyChangedEventArgs e);`  
  当任一依赖属性**有效值发生变化**时被调用。派生类可重写以**响应属性变化**（如联动其它属性、触发布局更新、状态切换等）。

- `protected internal virtual bool ShouldSerializeProperty(DependencyProperty dp);`  
  指示在 **序列化（如 XAML 序列化）** 时该属性是否应被序列化。可在派生类中控制持久化行为（通常用于设计器/持久化工具）。

---
### 2.` Visual `类

> `Visual` 是 **WPF 渲染系统的核心基类**，负责 2D 可视化树的呈现。绝大多数 UI 元素（如 `UIElement`、`FrameworkElement`）都是从它继承的。
>
> Visual类所做的事情只为控件呈现相关，但还不是去呈现控件,只是提供呈现的基础

---

#### 构造函数

- `protected Visual();`  
  受保护构造函数。仅允许派生类实例化（用户代码不能直接 `new Visual()`）。

---

#### 受保护/内部属性

这些属性都只读为了解Visual类的基础，因为这些属性都被设计成protected internal，我们的控件虽然继承了这个Visual类，但在实际的使用过程中是感知不到这些属性的，自然也不能实操它们。

- `protected DependencyObject VisualParent { get; }`  
  当前 `Visual` 的**父元素**。维护可视化树的层级关系。

- `protected virtual int VisualChildrenCount { get; }`  
  子元素数量。默认返回 0，派生类（如 `Panel`）会重写。

- `protected internal DoubleCollection VisualYSnappingGuidelines { get; protected set; }`  
  Y 方向的对齐基准线集合。用于像素对齐，避免模糊。

- `protected internal Vector VisualOffset { get; protected set; }`  
  可视元素相对于父级的偏移（平移变换）。

- `protected internal Geometry VisualClip { get; protected set; }`  
  定义裁剪区域的几何对象。用于限制绘制区域。

- `protected internal Rect? VisualScrollableAreaClip { get; protected set; }`  
  滚动区域的裁剪矩形（可空）。

- `protected internal CacheMode VisualCacheMode { get; protected set; }`  
  缓存模式，用于性能优化（如 `BitmapCache`）。

- `protected internal BitmapEffectInput VisualBitmapEffectInput { get; protected set; }`  
  **已过时**：位图特效输入。

- `protected internal BitmapEffect VisualBitmapEffect { get; protected set; }`  
  **已过时**：位图特效。

- `protected internal Effect VisualEffect { get; protected set; }`  
  WPF **Shader Effect**（如模糊、阴影）。

- `protected internal Transform VisualTransform { get; protected set; }`  
  视觉元素的 **变换矩阵**（平移/缩放/旋转）。

- `protected internal BitmapScalingMode VisualBitmapScalingMode { get; protected set; }`  
  位图缩放模式（如 `HighQuality`、`LowQuality`、`NearestNeighbor`）。

- `protected internal DoubleCollection VisualXSnappingGuidelines { get; protected set; }`  
  X 方向的对齐基准线集合。

- `protected internal double VisualOpacity { get; protected set; }`  
  不透明度，范围 `[0..1]`。

- `protected internal EdgeMode VisualEdgeMode { get; protected set; }`  
  边缘渲染模式（Aliased/Unspecified）。

- `protected internal ClearTypeHint VisualClearTypeHint { get; set; }`  
  ClearType 渲染提示。

- `protected internal TextRenderingMode VisualTextRenderingMode { get; set; }`  
  文本渲染模式（Aliased、ClearType、Grayscale）。

- `protected internal TextHintingMode VisualTextHintingMode { get; set; }`  
  文本渲染提示（影响小字体的清晰度）。

- `protected internal Brush VisualOpacityMask { get; protected set; }`  
  不透明度遮罩（按像素控制透明度，常用于渐变透明）。

---

#### 公共方法

- `public DependencyObject FindCommonVisualAncestor(DependencyObject otherVisual);`  
  查找当前元素与另一个可视元素的**最近公共祖先**。

- `public bool IsAncestorOf(DependencyObject descendant);`  
  判断当前元素是否是 `descendant` 的祖先。

- `public bool IsDescendantOf(DependencyObject ancestor);`  
  判断当前元素是否是 `ancestor` 的后代。

- `public Point PointFromScreen(Point point);`  
  将 **屏幕坐标** 转换为 **Visual 坐标**。

- `public Point PointToScreen(Point point);`  
  将 **Visual 坐标** 转换为 **屏幕坐标**。

- `public GeneralTransform2DTo3D TransformToAncestor(Visual3D ancestor);`  
  获取到 3D 祖先的二维到三维转换。

- `public GeneralTransform TransformToAncestor(Visual ancestor);`  
  获取到某个祖先的坐标变换。

- `public GeneralTransform TransformToDescendant(Visual descendant);`  
  获取到某个后代的坐标变换。

- `public GeneralTransform TransformToVisual(Visual visual);`  
  获取到另一个 Visual 的坐标变换。

---

#### 受保护方法（用于派生类）

- `protected void AddVisualChild(Visual child);`  
  将一个子元素添加到可视化树。**派生类必须成对调用 Add/Remove 管理子节点**。

- `protected virtual Visual GetVisualChild(int index);`  
  根据索引返回子元素。派生类必须重写以支持多个子元素。

- `protected virtual GeometryHitTestResult HitTestCore(GeometryHitTestParameters hitTestParameters);`  
  提供基于几何的命中测试。派生类可重写用于自定义命中逻辑。

- `protected virtual HitTestResult HitTestCore(PointHitTestParameters hitTestParameters);`  
  提供基于点的命中测试。

- `protected virtual void OnDpiChanged(DpiScale oldDpi, DpiScale newDpi);`  
  当 DPI 改变时调用。派生类可重写以调整绘制逻辑。

- `protected void RemoveVisualChild(Visual child);`  
  从可视化树移除一个子元素。

---

#### 受保护内部虚方法

- `protected internal virtual void OnVisualChildrenChanged(DependencyObject visualAdded, DependencyObject visualRemoved);`  
  当子元素集合发生变化时调用。

- `protected internal virtual void OnVisualParentChanged(DependencyObject oldParent);`  
  当父元素改变时调用。

---

### 3.`UIElement `类方法

> `UIElement` 是 **WPF 所有可见 UI 控件的基类**（如 `Button`、`TextBox`、`Panel` 等都继承它）。  
> 它在 `Visual` 的基础上增加了 **输入、焦点、布局、事件路由** 等核心功能。
>
> - `Visual` 更偏底层绘制，而 `UIElement` 则是 **交互与布局的最小单元**。  
> - 所有 WPF 控件都要经过 `Measure → Arrange → Render` 的生命周期，路由事件系统也是在 `UIElement` 里引入的。

---

#### 构造函数

- `public UIElement();`  
  创建 `UIElement` 实例（通常由派生类调用，不直接使用）。

---

#### 输入与焦点相关

- `public bool Focusable { get; set; }`  
  指示此元素是否可获得焦点。

- `public bool IsKeyboardFocused { get; }`  
  是否当前具有键盘焦点。

- `public bool IsKeyboardFocusWithin { get; }`  
  是否键盘焦点在该元素或其子元素内。

- `public bool IsMouseOver { get; }`  
  鼠标是否在元素上方。

- `public bool IsMouseDirectlyOver { get; }`  
  鼠标是否直接在元素上（不受子元素遮挡）。

- `public bool IsStylusOver { get; }`  
  是否手写笔指针在元素上。

- `public bool IsHitTestVisible { get; set; }`  
  是否参与命中测试（决定是否能响应鼠标事件）。

- `public bool IsEnabled { get; set; }`  
  元素是否启用（影响交互）。

---

#### 布局相关

- `public bool IsArrangeValid { get; }`  
  元素的 **Arrange 布局是否有效**。

- `public bool IsMeasureValid { get; }`  
  元素的 **Measure 布局是否有效**。

- `public bool IsVisible { get; }`  
  是否在可视树中并可见。

- `public Visibility Visibility { get; set; }`  
  控制显示/隐藏（Visible/Hidden/Collapsed）。

- `public double Opacity { get; set; }`  
  元素整体透明度 `[0..1]`。

- `public Transform RenderTransform { get; set; }`  
  渲染变换（平移/缩放/旋转/倾斜）。

- `public Point RenderTransformOrigin { get; set; }`  
  变换中心点（相对元素大小的百分比坐标）。

- `public CacheMode CacheMode { get; set; }`  
  缓存模式（如位图缓存）。

---

#### 输入事件（路由事件）

- `public event KeyEventHandler KeyDown;`  
  键盘按下事件。

- `public event KeyEventHandler KeyUp;`  
  键盘抬起事件。

- `public event MouseButtonEventHandler MouseDown;`  
  鼠标按下事件。

- `public event MouseButtonEventHandler MouseUp;`  
  鼠标抬起事件。

- `public event MouseEventHandler MouseMove;`  
  鼠标移动事件。

- `public event MouseWheelEventHandler MouseWheel;`  
  鼠标滚轮事件。

- `public event MouseEventHandler MouseEnter;`  
  鼠标进入元素。

- `public event MouseEventHandler MouseLeave;`  
  鼠标离开元素。

---

#### 命中测试与几何

- `public HitTestResult InputHitTest(Point point);`  
  在该元素范围内执行命中测试，返回命中结果。

- `protected override GeometryHitTestResult HitTestCore(GeometryHitTestParameters hitTestParameters);`  
  提供几何命中测试（继承自 Visual，UIElement 进行了实现）。

- `protected override HitTestResult HitTestCore(PointHitTestParameters hitTestParameters);`  
  提供点命中测试。

---

#### 焦点管理方法

- `public bool Focus();`  
  让此元素尝试获取键盘焦点。

- `public void MoveFocus(TraversalRequest request);`  
  移动焦点到相邻元素。

---

#### 布局方法（供布局系统调用）

- `protected virtual Size MeasureCore(Size availableSize);`  
  派生类重写以提供测量逻辑（计算元素所需大小）。

- `protected virtual Size ArrangeCore(Rect finalRect);`  
  派生类重写以安排元素的最终大小和位置。

---

#### 命令与输入绑定

- `public InputBindingCollection InputBindings { get; }`  
  输入绑定集合（键盘/鼠标手势 → 命令）。

- `public CommandBindingCollection CommandBindings { get; }`  
  命令绑定集合（命令 → 执行逻辑）。

---

#### 其他常用方法

- `public GeneralTransform TransformToVisual(Visual visual);`  
  获取到另一个 `Visual` 的坐标转换。

- `public void InvalidateMeasure();`  
  标记 Measure 无效，强制重新布局。

- `public void InvalidateArrange();`  
  标记 Arrange 无效，强制重新布局。

- `public void InvalidateVisual();`  
  标记渲染无效，强制重绘。

- `protected virtual void OnIsKeyboardFocusedChanged(DependencyPropertyChangedEventArgs e);`  
  当焦点状态变化时调用。

- `protected virtual void OnMouseDown(MouseButtonEventArgs e);`  
  鼠标按下时的默认处理逻辑。

---

### 4.`UIElement` 类主要属性

> `UIElement` 这些属性奠定了 WPF 控件的基础行为

---

#### 可见性与布局状态
- `bool IsArrangeValid { get; }`  
  指示元素的 **Arrange（排列）** 状态是否有效。

- `bool IsMeasureValid { get; }`  
  指示元素的 **Measure（测量）** 状态是否有效。

- `bool IsVisible { get; }`  
  元素是否可见（在可视树中且 `Visibility=Visible`）。

- `Visibility Visibility { get; set; }`  
  控制元素是否显示：  
  - `Visible`（显示）、  
  - `Hidden`（隐藏但仍占布局空间）、  
  - `Collapsed`（隐藏且不占布局空间）。

---

#### 输入与焦点
- `bool Focusable { get; set; }`  
  元素是否可获得焦点。

- `bool IsFocused { get; }`  
  元素是否拥有逻辑焦点。

- `bool IsKeyboardFocused { get; }`  
  元素是否拥有键盘焦点。

- `bool IsKeyboardFocusWithin { get; }`  
  键盘焦点是否在该元素或其子元素内部。

- `bool IsMouseOver { get; }`  
  鼠标是否在元素范围内（包含子元素区域）。

- `bool IsMouseDirectlyOver { get; }`  
  鼠标是否直接位于元素上（不包含子元素）。

- `bool IsStylusOver { get; }`  
  手写笔是否在元素上方。

- `bool IsStylusDirectlyOver { get; }`  
  手写笔是否直接位于元素上。

- `bool IsEnabled { get; set; }`  
  元素是否启用（禁用后不可交互）。

- `bool IsHitTestVisible { get; set; }`  
  元素是否能参与 **命中测试**（决定能否响应鼠标/触摸事件）。

---

#### 命令与输入绑定
- `InputBindingCollection InputBindings { get; }`  
  输入绑定集合（如快捷键绑定命令）。

- `CommandBindingCollection CommandBindings { get; }`  
  命令绑定集合（命令逻辑）。

---

#### 渲染与效果
- `double Opacity { get; set; }`  
  不透明度 `[0..1]`。

- `Transform RenderTransform { get; set; }`  
  渲染变换（缩放、旋转、平移等）。

- `Point RenderTransformOrigin { get; set; }`  
  变换中心点（以元素宽高为 1 的相对坐标）。

- `CacheMode CacheMode { get; set; }`  
  渲染缓存模式（如 `BitmapCache`）。

- `BitmapEffect BitmapEffect { get; set; }` **[已过时]**  
  旧的位图特效。

- `Effect Effect { get; set; }`  
  WPF Shader Effect（如模糊、阴影）。

- `BitmapEffectInput BitmapEffectInput { get; set; }` **[已过时]**  
  位图特效输入。

- `double OpacityMask { get; set; }`  
  不透明度遮罩（通常用 `Brush`，如渐变透明）。

---

#### 事件路由与输入处理
- `Dispatcher Dispatcher { get; }`  
  获取该元素关联的 **Dispatcher**（确保线程安全访问）。

- `DependencyObject Parent { get; }`  
  可视树中的父级。

- `bool AllowDrop { get; set; }`  
  是否允许拖放操作。

---

#### 其他
- `bool SnapsToDevicePixels { get; set; }`  
  是否启用像素对齐（避免边缘模糊）。

- `bool ClipToBounds { get; set; }`  
  是否将绘制裁剪到自身边界。

- `Geometry Clip { get; set; }`  
  裁剪几何图形（比 `ClipToBounds` 更灵活）。

- `Size DesiredSize { get; }`  
  布局测量阶段得到的“期望大小”。

- `UIElementCollection LogicalChildren { get; }`（在某些派生类中可用）  
  子元素集合。

---

### 5.`FrameworkElement` 主要属性

> `FrameworkElement` 是 `UIElement` 的子类，几乎所有常用控件（`Button`、`Grid`、`TextBox` 等）都继承它。  
> 在 `UIElement` 的基础上增加了 **布局属性、数据绑定、资源系统、样式、数据上下文** 等核心功能。

---

#### 1. 布局属性（Layout）

- `double Width { get; set; }`  
  元素的 **固定宽度**（若设置则优先级高于 `MinWidth`/`MaxWidth`）。

- `double Height { get; set; }`  
  元素的 **固定高度**。

- `double MinWidth { get; set; }`  
  最小宽度。

- `double MinHeight { get; set; }`  
  最小高度。

- `double MaxWidth { get; set; }`  
  最大宽度。

- `double MaxHeight { get; set; }`  
  最大高度。

- `double ActualWidth { get; }`  
  渲染完成后元素的实际宽度。

- `double ActualHeight { get; }`  
  渲染完成后元素的实际高度。

- `Thickness Margin { get; set; }`  
  外边距，影响布局。

- `HorizontalAlignment HorizontalAlignment { get; set; }`  
  水平对齐方式（`Left` / `Right` / `Center` / `Stretch`）。

- `VerticalAlignment VerticalAlignment { get; set; }`  
  垂直对齐方式。

---

#### 2. 可视树与逻辑树

- `object Parent { get; }`  
  元素的逻辑父元素。

- `ResourceDictionary Resources { get; set; }`  
  资源字典，定义可复用的 **样式、模板、画刷、数据等**。

- `Style Style { get; set; }`  
  控件样式。

- `object Tag { get; set; }`  
  自定义数据容器，可挂任意对象（类似 WinForms 的 Tag）。

---

#### 3. 数据绑定与上下文

- `object DataContext { get; set; }`  
  数据上下文，供数据绑定使用。通常在 `MVVM` 中设置为 ViewModel。

- `BindingGroup BindingGroup { get; set; }`  
  控制一组绑定的验证行为。

- `BindingExpression GetBindingExpression(DependencyProperty dp);`  
  获取某个属性的绑定表达式。

---

#### 4. 样式与主题

- `Style Style { get; set; }`  
  控件样式（定义外观）。

- `Style BaseStyle { get; }`  
  （间接通过 Style 继承）。

- `ControlTemplate Template { get; set; }`（在 Control 子类中可用）  
  控件模板，定义完整的控件外观结构。

---

#### 5. 可视化相关

- `Brush Background { get; set; }`  
  背景画刷（Panel/Control 常用）。

- `Brush Foreground { get; set; }`  
  前景画刷（文本颜色）。

- `FontFamily FontFamily { get; set; }`  
  字体。

- `double FontSize { get; set; }`  
  字号。

- `FontWeight FontWeight { get; set; }`  
  字重（Bold、Normal）。

- `FontStyle FontStyle { get; set; }`  
  字体样式（Italic、Normal）。

- `FontStretch FontStretch { get; set; }`  
  字体拉伸。

---

#### 6. 事件与生命周期

- `RoutedEventHandler Loaded;`  
  元素加载完成（加入可视树）。

- `RoutedEventHandler Unloaded;`  
  元素卸载（从可视树移除）。

- `SizeChangedEventHandler SizeChanged;`  
  元素尺寸变化时触发。

- `DependencyPropertyChangedEventHandler DataContextChanged;`  
  数据上下文变化时触发。

- `RequestBringIntoViewEventHandler RequestBringIntoView;`  
  请求将元素滚动到可见区域。

---

#### 7. 排版（FlowDirection）

- `FlowDirection FlowDirection { get; set; }`  
  内容流向（LeftToRight / RightToLeft，常用于国际化）。

- `Thickness Padding { get; set; }`（在 Control 中定义）  
  内边距。

---

#### 8. 工具提示与辅助

- `object ToolTip { get; set; }`  
  鼠标悬停提示。

- `Cursor Cursor { get; set; }`  
  鼠标指针样式。

- `ContextMenu ContextMenu { get; set; }`  
  右键上下文菜单。

---

### 6. `FrameworkElement`方法

> `FrameworkElement` 在 `UIElement` 的基础上，增加了 **布局、数据绑定、资源、样式、模板** 等关键方法。  
> 几乎所有常见控件（`Button`、`Grid`、`TextBox` 等）都依赖这些方法。

---

#### 1. 布局相关方法

- `public void InvalidateMeasure();`  
  标记此元素的 **测量结果无效**，会在下一次布局循环中重新调用 `Measure()`。

- `public void InvalidateArrange();`  
  标记此元素的 **排列结果无效**，会在下一次布局循环中重新调用 `Arrange()`。

- `public void UpdateLayout();`  
  强制立即执行一次 **布局更新**（避免延迟，通常用于需要立刻获取最新尺寸的场景）。

- `protected virtual Size MeasureOverride(Size availableSize);`  
  派生类重写，用于计算自身所需大小（Measure 阶段）。

- `protected virtual Size ArrangeOverride(Size finalSize);`  
  派生类重写，用于安排子元素在自身内部的位置与大小（Arrange 阶段）。

---

#### 2. 资源与样式

- `public object FindResource(object resourceKey);`  
  查找资源字典中的对象（递归向上查找）。如果未找到会抛异常。

- `public object TryFindResource(object resourceKey);`  
  查找资源，若未找到返回 `null`（更安全）。

- `public T FindName<T>(string name);`  
  在元素的 **名称范围（NameScope）** 中查找具有指定 `x:Name` 的元素。

- `public void SetResourceReference(DependencyProperty dp, object resourceKey);`  
  将某个依赖属性绑定到资源引用（支持动态资源）。

---

#### 3. 数据绑定

- `public BindingExpression GetBindingExpression(DependencyProperty dp);`  
  获取指定依赖属性上的绑定表达式对象。

- `public void SetBinding(DependencyProperty dp, BindingBase binding);`  
  为依赖属性设置数据绑定。

- `public BindingExpressionBase SetBinding(DependencyProperty dp, string path);`  
  使用字符串路径设置绑定（简化版本）。

---

#### 4. 数据上下文与查找

- `public object FindName(string name);`  
  在当前命名范围内查找具有指定 `x:Name` 的对象。

- `public DependencyObject FindLogicalNode(DependencyObject scope, string name);`  
  在逻辑树中查找指定名称的元素。

---

#### 5. 生命周期与事件

- `protected virtual void OnInitialized(EventArgs e);`  
  当元素初始化完成时调用。适合执行初始化逻辑。

- `protected virtual void OnPropertyChanged(DependencyPropertyChangedEventArgs e);`  
  当依赖属性值更改时调用（继承自 `DependencyObject`，在 `FrameworkElement` 可重写）。

- `protected virtual void OnRender(DrawingContext drawingContext);`  
  渲染当前元素的可视内容。派生类可重写以自定义绘制。

---

#### 6. 工具方法

- `public GeneralTransform TransformToVisual(Visual visual);`  
  获取从当前元素到另一个 `Visual` 的坐标转换。

- `public Point TranslatePoint(Point point, UIElement relativeTo);`  
  将一个点从当前元素坐标系转换到指定元素坐标系。

- `public bool ApplyTemplate();`  
  强制应用控件的模板（通常在修改 `Template` 后调用）。

---

# 布局控件

## 1.`Panel`(布局基类)

> - **自定义布局**：继承 `Panel` 并重写 `MeasureOverride`、`ArrangeOverride`，即可实现自定义布局（如环形、瀑布流等）。  
> - **Children 与 InternalChildren**：  
>   - `Children` 用于**开发者显式添加**的子元素；  
>   - `InternalChildren` 可能包含 **生成的容器**（如 `ItemsControl` 的项目容器），用于框架/派生类进行更底层的布局管理。  
> - **ZIndex**：默认值为0.通过 `Panel.SetZIndex(child, n)` 控制子元素前后层级；对命中测试（点击/拖拽,即鼠标点击事件优先命中的元素）也有影响。  
>   它决定了子元素在 **同一个 Panel（如 Grid、Canvas、StackPanel）内部的前后显示顺序**。
>   如果值相同则按声明顺序来决定前后
> - **IsItemsHost**：若该 `Panel` 由 `ItemsPresenter` 作为 `ItemsPanel` 实例化，则该属性为 `true`，面板应谨慎直接操作 `Children` —— 以免破坏 `ItemsControl` 的生成逻辑。  
> - **性能建议**：在批量更新子元素或属性时，尽量成组进行并减少不必要的 `InvalidateMeasure/Arrange`，以降低布局成本。

### `Panel`属性
#### 1. 子元素与宿主
- `public UIElementCollection Children { get; }`  
  **面向开发者**的子元素集合。通过 `Children.Add(...)`/`Remove(...)` 管理子元素。
- `protected internal UIElementCollection InternalChildren { get; }`  
  **框架内部/派生类**使用的子元素集合（包含生成的容器等，`ItemsControl` 模板化场景常用）。
- `public bool IsItemsHost { get; }`  
  指示此 `Panel` 是否作为 `ItemsControl` 的 **ItemsHost**（来自 `ItemsPresenter` 展开后承载数据项）。

#### 2. 外观与渲染
- `public Brush Background { get; set; }`  
  面板背景画刷。`Panel` 默认在 `OnRender` 中绘制背景（若不为空）。

#### 3. 附加属性（Attached Property）
- `public static int GetZIndex(UIElement element)`  
  获取子元素的 **Z 轴顺序**（越大越靠上）。
- `public static void SetZIndex(UIElement element, int value)`  
  设置子元素的 **Z 轴顺序**。  
  > ZIndex 只在同一父级内比较；改变后会影响渲染/命中顺序。

---

### `Panel` 方法

#### 1. 布局相关方法（派生类通常需要重写）
- `protected override Size MeasureOverride(Size availableSize)`  
  **测量阶段**：派生类应遍历 `Children` 调用 `child.Measure(...)`，并返回面板所需大小。
- `protected override Size ArrangeOverride(Size finalSize)`  
  **排列阶段**：派生类应遍历 `Children` 调用 `child.Arrange(...)`，根据自己的布局策略摆放子元素。

#### 2. 子元素管理（派生类可用）
- `protected virtual UIElementCollection CreateUIElementCollection(FrameworkElement logicalParent)`  
  创建 `UIElementCollection` 的工厂方法。需要自定义集合行为时可重写。
- `protected internal override void OnVisualChildrenChanged(DependencyObject visualAdded, DependencyObject visualRemoved)`  
  可视子元素增删时触发。派生类可在此联动状态（如失效布局、更新可视状态）。

#### 3. 渲染与尺寸变化
- `protected override void OnRender(DrawingContext dc)`  
  默认负责绘制 `Background`。派生类可重写以实现自定义绘制。
- `protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)`  
  面板尺寸变化时触发。可在此触发重新布局或缓存失效等逻辑。

#### 4. 序列化/设计期（可选）
- `public bool ShouldSerializeChildren()`  
  指示在 **XAML 序列化** 时是否应序列化 `Children` 集合（通常用于设计器与持久化流程）。

---

##  2. Grid(网格布局)

> 1. **说明** 
>
>    - `RowDefinition.Height` / `ColumnDefinition.Width` 支持三种模式默认使用Star(*)：  
>      
>      - `Auto`：按内容大小。  行高或列宽根据 **子元素的实际需要** 自动确定。
>      
>        **计算规则**：
>      
>        - Grid 在布局时，会先对 `Auto` 定义的行/列调用 `Measure()`。
>        - 行/列的大小 = **该行/列内所有子元素的 DesiredSize（期望大小）最大值**。
>        - 如果该行/列里只有一个控件，大小就等于该控件所需的大小。
>      
>      - `Pixel`（绝对值）：固定像素。  行高或列宽固定为指定的像素值。
>      
>        **计算规则**：
>      
>        - 无论子元素内容多大，该行/列始终是固定大小。
>        - 如果子元素比它大 → 元素会被裁剪或压缩。
>        - 如果子元素比它小 → 会留空白。
>      
>      - `Star(*)`：按比例分配剩余空间。  将容器的剩余空间按比例分配给使用 `*` 的行/列。
>      
>        **计算规则**：
>      
>        1. 先给 `Auto` 和 `Pixel` 分配空间。
>        2. 剩下的空间按 `*` 权重分配。
>        3. `*` = 1 份，`2*` = 2 份，依此类推。
>    
>    例如：
>    ```xml
>    <Grid>
>        <Grid.RowDefinitions>
>            <RowDefinition Height="Auto"/>
>            <RowDefinition Height="100"/>
>            <RowDefinition Height="2*"/>
>        </Grid.RowDefinitions>
>    </Grid>

### `Grid` 属性

#### 1. 行与列定义
- `public RowDefinitionCollection RowDefinitions { get; }`  
  获取或设置网格的行定义集合（`RowDefinition` 对象集合，用于控制行的数量与大小）。
- `public ColumnDefinitionCollection ColumnDefinitions { get; }`  
  获取或设置网格的列定义集合（`ColumnDefinition` 对象集合，用于控制列的数量与大小）。
- `public static readonly DependencyProperty ShowGridLinesProperty`  
  依赖属性：控制是否显示网格线。
- `public bool ShowGridLines { get; set; }`  
  是否在调试时显示辅助线（仅用于开发调试，不建议在生产 UI 使用）。

#### 2. 附加属性（Attached Properties，用于子元素）
- `public static int GetRow(UIElement element)`  
  获取子元素所在的行索引。
- `public static void SetRow(UIElement element, int value)`  
  设置子元素所在的行索引。

- `public static int GetColumn(UIElement element)`  
  获取子元素所在的列索引。
- `public static void SetColumn(UIElement element, int value)`  
  设置子元素所在的列索引。

- `public static int GetRowSpan(UIElement element)`  
  获取子元素跨越的行数。
- `public static void SetRowSpan(UIElement element, int value)`  
  设置子元素跨越的行数。

- `public static int GetColumnSpan(UIElement element)`  
  获取子元素跨越的列数。
- `public static void SetColumnSpan(UIElement element, int value)`  
  设置子元素跨越的列数。

---

### `Grid` 方法

#### 1. 布局相关（重写自 `Panel`）
- `protected override Size MeasureOverride(Size constraint)`  
  测量子元素大小。`Grid` 会根据 `RowDefinitions` 与 `ColumnDefinitions` 分配可用空间并调用子元素的 `Measure`。
- `protected override Size ArrangeOverride(Size arrangeSize)`  
  安排子元素位置。`Grid` 会根据计算出的行列边界调用子元素的 `Arrange`。

#### 2. 行列定义处理（内部使用，派生类扩展可参考）
- `protected override void OnRender(DrawingContext dc)`  
  默认实现会在 `ShowGridLines = true` 时绘制辅助网格线。

---

## 3.`UniformGrid`（均分布局）
> 
1. **等分布局**  
   - `UniformGrid` 会将所有子元素放置到一个均匀的行列网格中。  
   - 每个单元格的大小相等（宽度一致，高度一致）。  
   - 与 `Grid` 不同，它不支持 `RowSpan` / `ColumnSpan`，也没有 `*` 比例定义。
2. **自动推断行列**  
        - 如果只设置 `Rows`，则列数会根据子元素数量自动推断。  
        - 如果只设置 `Columns`，则行数会根据子元素数量自动推断。  
        - 如果都设置了，则严格按照设定的行列布局，超出的元素继续填充。
3. **FirstColumn 的用途**  
        - 可以指定第一行从第几列开始，前面列留空。  
            - 例如在日历控件中，第一周可能从周三开始 → `FirstColumn=2`。


### `UniformGrid` 属性

#### 1. 行列控制
- `public int Rows { get; set; }`  
  指定网格的行数。默认值 `0`，表示未显式设置（会根据子元素数量和 Columns 自动推断）。

- `public int Columns { get; set; }`  
  指定网格的列数。默认值 `0`，表示未显式设置（会根据子元素数量和 Rows 自动推断）。

- `public int FirstColumn { get; set; }`  
  指定子元素从第几列开始布局。默认值 `0`，即从第一列开始。  
  典型用途：在布局第一行时跳过前几列。

---

### `UniformGrid` 方法

#### 1. 布局重写（继承自 `Panel`）
- `protected override Size MeasureOverride(Size availableSize)`  
  计算并测量所有子元素。  
  `UniformGrid` 会根据 `Rows` 与 `Columns` 计算每个单元格大小，并对每个子元素调用 `Measure`。

- `protected override Size ArrangeOverride(Size finalSize)`  
  安排所有子元素在网格中的位置。  
  每个子元素都被分配到相同大小的单元格中，位置由行列索引决定。




---

##　４.StackPanel（栈式布局）

> 1. **自动堆叠布局**  
>    - 子元素按顺序依次堆叠。  
>    - 常用于菜单栏、按钮组、表单行。  
>
> 2. **空间占用**  
>    - `Vertical` 模式下：  
>      - 高度 = 所有子元素高度之和。  
>      - 宽度 = 子元素中最宽的一个。  
>    - `Horizontal` 模式下：  
>      - 宽度 = 所有子元素宽度之和。  
>      - 高度 = 子元素中最高的一个。  
>
> 3. **与 `WrapPanel` 区别**  
>    - `StackPanel` 只会单行/单列排列，超出边界不会换行。  
>    - `WrapPanel` 会自动换行/换列。  
>
> 4. **性能优势**  
>    - `StackPanel` 的布局逻辑比 `Grid` 更轻量，适合简单顺序排列场景。  
>    - 如果只是单一纵向或横向排布，优先使用 `StackPanel`。  
>
> ---
>
> ### 示例
>
> ```xml
> <StackPanel Orientation="Vertical" Background="LightGray" Margin="10">
>     <Button Content="按钮 1" Height="40" Margin="5"/>
>     <Button Content="按钮 2" Height="40" Margin="5"/>
>     <Button Content="按钮 3" Height="40" Margin="5"/>
> </StackPanel>

### `StackPanel` 属性

#### 1. 排列方向
- `public Orientation Orientation { get; set; }`  
  控件排列方向：  
  - `Horizontal` → 子元素水平排列（从左到右）。  
  - `Vertical`（默认）→ 子元素垂直排列（从上到下）。  

#### 2. 子元素对齐与布局继承（来自 `Panel`/`FrameworkElement`）
- `public Brush Background { get; set; }`  
  背景画刷。  
- `public Thickness Margin { get; set; }`  
  外边距。  
- `public Thickness Padding { get; set; }`（在 `Control` 子类才出现，这里仅 Panel 水平控制 Margin）。  
- `public HorizontalAlignment HorizontalAlignment { get; set; }`  
  水平对齐方式。  Left表示显示在左则，Right显示在右则，Center则居中显示，Stretch表示拉伸填充显示。
- `public VerticalAlignment VerticalAlignment { get; set; }`  
  垂直对齐方式。  Left表示显示在左则，Right显示在右则，Center则居中显示，Stretch表示拉伸填充显示。

---

### `StackPanel` 方法

#### 1. 布局重写
- `protected override Size MeasureOverride(Size constraint)`  
  测量子元素大小。`StackPanel` 会遍历 `Children`，根据 `Orientation` 决定累加 **宽度或高度**。
  
  - `Vertical`：累加高度，取最大宽度作为整体宽度。  
  - `Horizontal`：累加宽度，取最大高度作为整体高度。  
  
- `protected override Size ArrangeOverride(Size arrangeSize)`  
  安排子元素位置。`StackPanel` 会顺序摆放：  
  
  - `Vertical`：每个元素在上一元素下方排列。  
  
  - `Horizontal`：每个元素在前一个元素右侧排列。  
  
    
## 5. WrapPanel（瀑布流布局）

> 1. **自动换行/换列布局**  
>    - 子元素依次排列，当主方向空间不足时会自动换行（或换列）。  
>    - 常用于图片展示、标签面板、工具按钮栏。  
> 2. **空间占用**  
>    - `Orientation="Horizontal"`（默认）：  
>      - 水平方向依次排列，超出容器宽度时换行。  
>      - 容器高度 = 所有行的总高度。  
>    - `Orientation="Vertical"`：  
>      - 垂直方向依次排列，超出容器高度时换列。  
>      - 容器宽度 = 所有列的总宽度。  
> 3. **与 `StackPanel` 区别**  
>    - `StackPanel` 只会单行/单列堆叠，控件可能溢出。  
>    - `WrapPanel` 支持自动换行/换列，更灵活。  
> 4. **性能与使用场景**  
>    - 适合内容量不固定的场景，如：标签云、图片列表。  
>    - 比 `Grid` 简单，布局性能较好。  
>
> ---
>
> ### 示例
>
> ```xml
> <WrapPanel Orientation="Horizontal" Background="Beige" Margin="10">
>     <Button Content="按钮 1" Width="80" Height="30" Margin="5"/>
>     <Button Content="按钮 2" Width="80" Height="30" Margin="5"/>
>     <Button Content="按钮 3" Width="80" Height="30" Margin="5"/>
>     <Button Content="按钮 4" Width="80" Height="30" Margin="5"/>
>     <Button Content="按钮 5" Width="80" Height="30" Margin="5"/>
> </WrapPanel>
> ```
> 👉 当窗口变窄时，按钮会自动换行显示。

---

### `WrapPanel` 属性

#### 1. 排列方向
- `public Orientation Orientation { get; set; }`  
  控件排列方向：  
  - `Horizontal`（默认） → 子元素水平排列，超出宽度换行。  
  - `Vertical` → 子元素垂直排列，超出高度换列。  

#### 2. 子元素大小控制
- `public double ItemWidth { get; set; }`  
  子元素统一宽度。未设置时使用子元素自身的 `Width`。  
- `public double ItemHeight { get; set; }`  
  子元素统一高度。未设置时使用子元素自身的 `Height`。  

---

### `WrapPanel` 方法

#### 1. 布局重写
- `protected override Size MeasureOverride(Size constraint)`  
  测量子元素大小，考虑 `Orientation` 和 `ItemWidth`/`ItemHeight`，并计算需要的换行/换列数。  

- `protected override Size ArrangeOverride(Size finalSize)`  
  按行/列排列子元素，当主方向空间不足时，自动换到新的一行或一列。  

---
## 6. DockPanel（停靠布局）

> 1. **停靠布局原理**  
>    - 子元素可以停靠在容器的上、下、左、右边。  
>    - 最后一个未设置 `Dock` 的子元素会自动填充剩余空间。  
>
> 2. **空间占用与排列顺序**  
>    - 子元素按 **声明顺序** 依次布置，先声明的先停靠。  
>    - 若多个子元素都停靠同一边，则它们会依次叠加。  
>
> 3. **与 `Grid`、`StackPanel` 区别**  
>    - `Grid` → 灵活的二维表格布局。  
>    - `StackPanel` → 单方向堆叠，不会填充剩余空间。  
>    - `DockPanel` → 多边停靠 + 剩余空间填充，适合主界面分区。  
>
> 4. **典型使用场景**  
>    - 主窗口框架布局：顶部菜单栏、左侧导航栏、底部状态栏、中间工作区。  
>    - 工具栏停靠布局。  
>
> ---
>
> ### 示例
>
> ```xml
> <DockPanel LastChildFill="True">
>     <!-- 顶部菜单栏 -->
>     <Menu DockPanel.Dock="Top">
>         <MenuItem Header="文件"/>
>         <MenuItem Header="编辑"/>
>     </Menu>
>
>     <!-- 左侧导航栏 -->
>     <StackPanel DockPanel.Dock="Left" Background="LightGray" Width="120">
>         <Button Content="功能 1"/>
>         <Button Content="功能 2"/>
>     </StackPanel>
>
>     <!-- 底部状态栏 -->
>     <StatusBar DockPanel.Dock="Bottom" Background="Beige">
>         <TextBlock Text="状态：就绪"/>
>     </StatusBar>
>
>     <!-- 主内容区（自动填充剩余空间） -->
>     <Grid Background="White"/>
> </DockPanel>
> ```
> 👉 效果：菜单栏在上，导航在左，状态栏在下，中间 Grid 自动填充剩余空间。

---

### `DockPanel` 属性

#### 1. 停靠控制
- `public static Dock GetDock(UIElement element)`  
  获取指定子元素的停靠方向（Top/Bottom/Left/Right）。  

- `public static void SetDock(UIElement element, Dock dock)`  
  设置子元素的停靠方向。  

- `public bool LastChildFill { get; set; }`  
  是否让最后一个子元素自动填充剩余空间。默认 `true`。  

---

### `DockPanel` 方法

#### 1. 布局重写
- `protected override Size MeasureOverride(Size constraint)`  
  遍历子元素并调用 `Measure`，计算总大小，考虑停靠方向。  

- `protected override Size ArrangeOverride(Size arrangeSize)`  
  按顺序停靠子元素：  
  - `Top/Bottom` → 在容器顶部/底部堆叠。  
  - `Left/Right` → 在容器左/右堆叠。  
  - 最后一个子元素（若 `LastChildFill=true`）→ 占满剩余空间。  


## 7. VirtualizingStackPanel（虚拟化栈式布局）

> 1. **虚拟化原理**  
>    - 继承自 `StackPanel`，但支持 **UI 虚拟化**：  
>      - 只为**可见区域内的子元素**创建和渲染控件。  
>      - 不可见的元素不会立即生成，等滚动到视区时才生成。  
>    - 主要用于 `ListBox`、`ListView`、`ItemsControl` 等控件的 `ItemsPanel`。  
>
> 2. **性能优势**  
>    - 当数据量很大（成千上万条）时，普通 `StackPanel` 会一次性创建所有子控件，内存和性能消耗严重。  
>    - `VirtualizingStackPanel` 则按需创建，大大减少 **内存占用和布局计算**，保证滚动流畅。  
>
> 3. **使用场景**  
>    - 数据量大、需要滚动显示的控件：如文件列表、日志视图、聊天窗口。  
>    - 在 `ItemsControl` 中作为 `ItemsPanelTemplate`。  
>
> ---
>
> ### 示例
>
> ```xml
> <ListBox>
>     <ListBox.ItemsPanel>
>         <ItemsPanelTemplate>
>             <VirtualizingStackPanel Orientation="Vertical"/>
>         </ItemsPanelTemplate>
>     </ListBox.ItemsPanel>
> </ListBox>
> ```
> 👉列表使用虚拟化栈式布局，滚动时才加载可见项，提升性能。

---

### `VirtualizingStackPanel` 属性

#### 1. 布局方向
- `public Orientation Orientation { get; set; }`  
  子元素排列方向：  
  - `Vertical`（默认） → 垂直堆叠。  
  - `Horizontal` → 水平堆叠。  

#### 2. 虚拟化控制（附加属性）
- `public static bool GetIsVirtualizing(DependencyObject o)`  
  获取是否启用虚拟化。  
- `public static void SetIsVirtualizing(DependencyObject o, bool value)`  
  设置是否启用虚拟化。  

- `public static VirtualizationMode GetVirtualizationMode(DependencyObject o)`  
  获取虚拟化模式：  
  - `Standard`（标准虚拟化，回收不可见容器）。  
  - `Recycling`（回收并复用容器，性能更高）。  
- `public static void SetVirtualizationMode(DependencyObject o, VirtualizationMode value)`  
  设置虚拟化模式。  

- `public static bool GetIsVirtualizingWhenGrouping(DependencyObject o)`  
  获取分组时是否启用虚拟化。  
- `public static void SetIsVirtualizingWhenGrouping(DependencyObject o, bool value)`  
  设置分组时是否启用虚拟化。  

- `public static bool GetScrollUnit(DependencyObject o)`  
  获取滚动单位（`Pixel` 或 `Item`）。  
- `public static void SetScrollUnit(DependencyObject o, ScrollUnit value)`  
  设置滚动单位。  

---

### `VirtualizingStackPanel` 方法

#### 1. 布局重写
- `protected override Size MeasureOverride(Size constraint)`  
  测量可见区域子元素大小，虚拟化不可见项。  

- `protected override Size ArrangeOverride(Size arrangeSize)`  
  安排可见子元素位置，按需更新虚拟化缓存。  

#### 2. 滚动支持
- `public void BringIndexIntoViewPublic(int index)`  
  滚动到指定索引的项并确保可见。  

---

## 8. Canvas（绝对定位布局）

> 1. **绝对定位原理**  
>    - 子元素使用 **坐标属性**（`Left`、`Top`、`Right`、`Bottom`）进行定位。  
>    - 与 `Grid`、`StackPanel` 等自动布局不同，`Canvas` 不会自动调整位置，开发者完全掌控。  
>2. **空间占用与排列规则**  
>    - 子元素的坐标相对于 **Canvas 左上角**。  
>    - 若同时设置 `Left` 与 `Right`，则优先 `Left`；若同时设置 `Top` 与 `Bottom`，则优先 `Top`。  
>    - 未设置时，元素会堆叠在左上角。  
> 4. **典型使用场景**  
>   - 自定义图形绘制。  
>    - 拖拽 UI（拖放文件、可视化编辑器）。  
>    - 动画场景（Storyboard 与 `Canvas.Left/Top` 配合）。  
> 
> ---
> 
>### 示例
> 
> ```xml
> <Canvas Background="Beige">
>     <Rectangle Width="80" Height="50" Fill="Red"
>               Canvas.Left="30" Canvas.Top="20"/>
> 
>    <Ellipse Width="60" Height="60" Fill="Blue"
>              Canvas.Left="150" Canvas.Top="100"/>
>
>     <Button Content="按钮" Width="100" Height="40"
>             Canvas.Right="20" Canvas.Bottom="20"/>
> </Canvas>
> ```
>👉 效果：  
> - 红色矩形在 `(30,20)`。  
> - 蓝色圆形在 `(150,100)`。  
>- 按钮停靠在右下角。  

---

### `Canvas` 属性

#### 1. 定位属性（附加属性）
- `public static double GetLeft(UIElement element)`  
  获取子元素相对 `Canvas` 左边的距离。  
- `public static void SetLeft(UIElement element, double length)`  
  设置子元素相对 `Canvas` 左边的距离。  

- `public static double GetTop(UIElement element)`  
  获取子元素相对 `Canvas` 顶边的距离。  
- `public static void SetTop(UIElement element, double length)`  
  设置子元素相对 `Canvas` 顶边的距离。  

- `public static double GetRight(UIElement element)`  
  获取子元素相对 `Canvas` 右边的距离。  
- `public static void SetRight(UIElement element, double length)`  
  设置子元素相对 `Canvas` 右边的距离。  

- `public static double GetBottom(UIElement element)`  
  获取子元素相对 `Canvas` 底边的距离。  
- `public static void SetBottom(UIElement element, double length)`  
  设置子元素相对 `Canvas` 底边的距离。  

---

### `Canvas` 方法

#### 1. 布局重写
- `protected override Size MeasureOverride(Size constraint)`  
  只测量子元素本身大小，不进行自动排列。  

- `protected override Size ArrangeOverride(Size arrangeSize)`  
  按 `Left`/`Top`/`Right`/`Bottom` 定位子元素，未设置时按 `(0,0)` 放置。  
  
  
  

---
## 9. Border（边框布局）

> 1. **边框容器原理**  
>    - `Border` 是一个 **单子元素容器**，只能包含一个子元素。  
>    - 它的主要作用是：绘制边框、背景、圆角效果，并为子元素提供 **Padding 内边距**。  
>
> 2. **常见用途**  
>    - 给控件或布局容器加上边框和背景。  
>    - 做卡片式 UI、按钮边框、提示框。  
>    - 配合 `CornerRadius` 制作圆角矩形效果。  
>
> 3. **与其他容器区别**  
>    - `Panel` 可以包含多个子元素，而 `Border` **只能包含一个**。  
>    - 常用于“装饰”一个控件或容器。  
>
> ---
>
> ### 示例
>
> ```xml
> <Border BorderBrush="DarkGray" BorderThickness="2"
>         Background="LightYellow"
>         CornerRadius="10" Padding="15" Margin="10">
>     <TextBlock Text="这是带边框的文本"
>                FontSize="16" Foreground="Black"/>
> </Border>
> ```
> 👉 效果：  
> - 浅黄色背景，灰色边框。  
> - 四角圆角半径为 10。  
> - 内边距 15，子文本与边框保持间距。  

---

### `Border` 属性

#### 1. 外观
- `public Brush BorderBrush { get; set; }`  
  边框画刷。  

- `public Thickness BorderThickness { get; set; }`  
  边框厚度（可分别设置上下左右）。  

- `public Brush Background { get; set; }`  
  背景填充画刷。  

- `public CornerRadius CornerRadius { get; set; }`  
  圆角半径。可分别设置四个角。  

#### 2. 内边距
- `public Thickness Padding { get; set; }`  
  内边距，子元素与边框之间的空间。  

#### 3. 子元素
- `public UIElement Child { get; set; }`  
  包含的单个子元素。  

---

### `Border` 方法

#### 1. 渲染相关
- `protected override void OnRender(DrawingContext dc)`  
  渲染边框和背景。  
  - 根据 `BorderBrush` 和 `BorderThickness` 绘制边框。  
  - 根据 `Background` 绘制填充区域。  
  - 根据 `CornerRadius` 绘制圆角矩形。  

#### 2. 布局相关
- `protected override Size MeasureOverride(Size constraint)`  
  测量子元素大小，加上 `Padding` 与边框厚度。  

- `protected override Size ArrangeOverride(Size finalSize)`  
  将子元素安排到边框内部区域（考虑内边距和边框）。  



# 内容控件

## 1. Control（基类）

> 1. **基类作用**  
>    - `Control` 继承自 `FrameworkElement`，是 WPF **绝大多数可交互控件（Button、TextBox、ListBox 等）** 的基类。  
>    - 在 `FrameworkElement` 的布局、资源、数据绑定基础上，`Control` 增加了 **样式、模板、视觉状态、内容呈现** 等能力。  
>
> 2. **主要职责**  
>    - 提供 `Template`（控件外观定义）。  
>    - 支持 `Foreground`、`Background`、`Padding`、`BorderBrush` 等常见属性。  
>    - 支持 `FocusVisualStyle`、`IsTabStop` 等交互属性。  
>    - 提供 `DefaultStyleKey`，允许自定义控件外观。  
>
> 3. **使用场景**  
>    - 所有需要 **可视化呈现 + 用户交互** 的控件都基于 `Control`。  
>    - 自定义控件时通常从 `Control` 派生，自己写 `ControlTemplate` 定义外观。  
>
> ---
>
> ### 示例
>
> ```xml
> <Button Content="Hello WPF" 
>         Background="LightBlue"
>         Foreground="DarkBlue"
>         Padding="10"
>         BorderBrush="Gray"
>         BorderThickness="2"/>
> ```
> 👉 `Button` 继承自 `Control`，因此具备上述属性。  

---

### `Control` 属性

#### 1. 外观与样式
- `public Brush Background { get; set; }`  
  控件背景画刷。  

- `public Brush BorderBrush { get; set; }`  
  边框画刷。  

- `public Thickness BorderThickness { get; set; }`  
  边框厚度。  

- `public Brush Foreground { get; set; }`  
  前景画刷（文字颜色）。  

- `public Thickness Padding { get; set; }`  
  内边距，内容与边框之间的空隙。  

- `public Style FocusVisualStyle { get; set; }`  
  当控件获得焦点时的样式（比如虚线框）。  

- `public ControlTemplate Template { get; set; }`  
  控件模板，定义控件的视觉结构。  

- `public object DefaultStyleKey { get; }`  
  用于查找默认样式的键。自定义控件时可覆盖。  

#### 2. 输入与焦点
- `public bool IsTabStop { get; set; }`  
  控件是否可以通过 Tab 获得焦点。  

- `public int TabIndex { get; set; }`  
  控件的 Tab 顺序。  

#### 3. 字体与排版（继承自 `FrameworkElement`，在 Control 中直接使用）
- `public FontFamily FontFamily { get; set; }`  
  字体。  
- `public double FontSize { get; set; }`  
  字号。  
- `public FontWeight FontWeight { get; set; }`  
  字重（Bold/Normal）。  
- `public FontStyle FontStyle { get; set; }`  
  字体样式（Italic/Normal）。  
- `public FontStretch FontStretch { get; set; }`  
  字体拉伸。  

---

### `Control` 方法

#### 1. 模板相关
- `public void ApplyTemplate()`  
  强制应用模板，确保 `Template` 已加载并可以通过 `GetTemplateChild` 查找子元素。  

- `protected virtual void OnTemplateChanged(ControlTemplate oldTemplate, ControlTemplate newTemplate)`  
  当 `Template` 改变时调用。  

- `protected DependencyObject GetTemplateChild(string childName)`  
  获取模板中的命名元素。  

#### 2. 外观渲染
- `protected override void OnRender(DrawingContext drawingContext)`  
  渲染控件的背景、边框等内容。  

- `protected virtual void OnGotFocus(RoutedEventArgs e)`  
  当控件获得焦点时调用。  

- `protected virtual void OnLostFocus(RoutedEventArgs e)`  
  当控件失去焦点时调用。  

---

## 2. ContentControl（内容控件基类）

> 1. **基类作用**  
>    - `ContentControl` 继承自 `Control`，增加了 **承载单一内容** 的能力。  
>    - 内容可以是 **字符串、UI 元素、数据对象**，并可通过 `ContentTemplate` 定义内容的显示方式。  
>
> 2. **主要职责**  
>    - 提供 `Content` 属性，允许开发者放置任意对象作为子元素。  
>    - 支持 `ContentTemplate` / `ContentTemplateSelector` 控制内容展示。  
>    - 常见控件如 `Button`、`Label`、`CheckBox`、`GroupBox` 都继承自它。  
>
> ---
>
> ### 示例
>
> ```xml
> <Button>
>     <StackPanel Orientation="Horizontal">
>         <Image Source="icon.png" Width="16" Height="16"/>
>         <TextBlock Text="按钮文字" Margin="5,0,0,0"/>
>     </StackPanel>
> </Button>
> ```
> 👉 因为 `Button` 继承自 `ContentControl`，它的 `Content` 可以是复杂的 UI 结构，而不仅仅是字符串。

---

### `ContentControl` 属性

#### 1. 内容相关
- `public object Content { get; set; }`  
  控件的内容，可以是字符串、数字、UIElement 或任意对象。  

- `public DataTemplate ContentTemplate { get; set; }`  
  定义内容的显示方式（数据模板）。  

- `public DataTemplateSelector ContentTemplateSelector { get; set; }`  
  用于动态选择 `ContentTemplate`。  

- `public string ContentStringFormat { get; set; }`  
  格式化内容的字符串（例如日期、数字格式）。  

#### 2. 内容呈现
- `public ContentPresenter ContentPresenter { get; }`（通过模板内部）  
  实际呈现内容的元素，通常在 `ControlTemplate` 中定义。  

---

### `ContentControl` 方法

#### 1. 生命周期与模板
- `protected override void OnContentChanged(object oldContent, object newContent)`  
  当 `Content` 属性发生变化时调用，派生类可重写。  

- `protected virtual void OnContentTemplateChanged(DataTemplate oldContentTemplate, DataTemplate newContentTemplate)`  
  当 `ContentTemplate` 改变时调用。  

- `protected virtual void OnContentTemplateSelectorChanged(DataTemplateSelector oldContentTemplateSelector, DataTemplateSelector newContentTemplateSelector)`  
  当 `ContentTemplateSelector` 改变时调用。  
  
  
## 3. ButtonBase（按钮基类）

> 1. **基类作用**  
>    - `ButtonBase` 继承自 `ContentControl`，是所有按钮类控件的基类（如 `Button`、`RepeatButton`、`ToggleButton`、`CheckBox`、`RadioButton`）。  
>    - 在 `ContentControl` 的内容显示功能上，增加了 **点击、命令绑定、按下/抬起等交互功能**。  
>
> 2. **主要职责**  
>    - 提供鼠标/键盘点击事件（`Click`）。  
>    - 支持命令（`Command`、`CommandParameter`、`CommandTarget`）。  
>    - 管理按钮的可点击状态（`IsPressed`、`ClickMode`）。  
>
> ---
>
> ### 示例
>
> ```xml
> <Button Content="提交"
>         Command="{Binding SubmitCommand}"
>         CommandParameter="UserData"
>         Width="100" Height="40"/>
> ```
> 👉 当按钮被点击时，会触发绑定的 `SubmitCommand`，并传递参数 `"UserData"`。

---

### `ButtonBase` 属性

#### 1. 命令相关
- `public ICommand Command { get; set; }`  
  要执行的命令，通常绑定到 ViewModel 的 `ICommand` 实现。  

- `public object CommandParameter { get; set; }`  
  命令执行时传递的参数。  

- `public IInputElement CommandTarget { get; set; }`  
  命令的目标对象（通常用于 `RoutedCommand`）。  

#### 2. 状态相关
- `public bool IsPressed { get; }`  
  指示按钮当前是否处于按下状态。  

- `public ClickMode ClickMode { get; set; }`  
  定义何时触发 `Click` 事件：  
  - `Release`（默认）：鼠标释放时触发。  
  - `Press`：鼠标按下时立即触发。  
  - `Hover`：鼠标悬停时触发。  

---

#### `ButtonBase` 事件

- `public event RoutedEventHandler Click;`  
  在按钮被单击时触发。  

---

#### `ButtonBase` 方法

##### 1. 事件触发
- `protected virtual void OnClick()`  
  引发 `Click` 事件。派生类可重写以自定义点击行为。  

##### 2. 输入交互
- `protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)`  
  鼠标左键按下时调用，设置 `IsPressed=true` 并可能触发命令。  

- `protected override void OnMouseLeftButtonUp(MouseButtonEventArgs e)`  
  鼠标左键释放时调用，若 `IsPressed=true` 则触发 `Click`。  

- `protected override void OnKeyDown(KeyEventArgs e)`  
  键盘按键时调用（如 `Space` 或 `Enter` 会触发点击）。  

- `protected override void OnKeyUp(KeyEventArgs e)`  
  键盘按键释放时调用，配合 `ClickMode` 判断是否触发点击。  

---

# 数据绑定

> 数据绑定 = 把 UI（控件属性）和数据（对象属性）用“通道”连起来，让它们自动保持同步。

---

### 1.MVVM 模式(Model-View-ViewModel Mode)

> View 不直接操作 Model，而是通过 DataContext 绑定到 ViewModel，由 ViewModel 来间接管理 Model。
>

---

#### 1.Model(模型层)

> **数据 & 业务逻辑** 的核心部分。
>
> 只关心数据结构和业务，不关心 UI。
>
> 可以是 POCO 类，也可以是实体类、数据库对象、API 返回对象。

示例一个User类,这是纯数据，和 UI 毫无关系。

```c#
public class User
{
    public string Name { get; set; }
    public int Age { get; set; }
}

```

#### 2.View(示图层)

> 就是 **XAML 页面**，负责显示界面。
>
> 理想情况下，View 不写业务逻辑，而是通过 **数据绑定 (Binding)** 来展示 ViewModel 提供的数据。
>
> 事件逻辑也尽量不写在 Code-behind(既XAML对应的后台代码文件)，而交给 ViewModel 的命令处理。

示例

```xml
<TextBlock Text="{Binding UserName}" />
<TextBox Text="{Binding Age, Mode=TwoWay}" />
<Button Content="保存" Command="{Binding SaveCommand}" />

```

#### 3.ViewModel（视图模型层）

> 连接 **View** 和 **Model** 的桥梁。
>
> 主要职责：
>
> - 把 **Model 的数据包装成属性**，供 View 绑定。
> - 通过 **INotifyPropertyChanged** 通知 UI 更新。
> - 提供 **命令 (ICommand)**，响应 UI 操作（比如按钮点击）。
>
> 不依赖具体的 View，可以单独测试。

示例

```c#
public class UserViewModel : INotifyPropertyChanged
{
    private string _userName;
    public string UserName
    {
        get => _userName;
        set { _userName = value; RaisePropertyChanged(); }
    }

    public ICommand SaveCommand { get; }
    public User Model { get; }

    public UserViewModel(User user)
    {
        Model = user;
        _userName = user.Name;
        SaveCommand = new RelayCommand(Save);
    }

    private void Save() => Console.WriteLine("保存数据：" + UserName);

    public event PropertyChangedEventHandler PropertyChanged;
    protected void RaisePropertyChanged([CallerMemberName] string propName = null)
        => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propName));
}

```





### 2.DataContext（数据上下文）

> 定义:DataContext = 数据绑定时默认查找的数据源。(告诉 View，它要绑定哪个 ViewModel。)
>
> 如<TextBlock Text="{Binding UserName}" />
>
> `Binding` 并没有指定 `Source`，那 WPF 就会去找它的 **DataContext**，并在里面查找 `UserName` 属性。

---



####　1.继承机制

**DataContext 会从父元素自动继承到子元素**。

子控件也可以重新设置自己的 DataContext，这会覆盖继承的值。

示例:

```xml
<Window>
    <Window.DataContext>
        <local:UserViewModel />
    </Window.DataContext>

    <StackPanel>
        <TextBlock Text="{Binding UserName}" />
        <TextBox Text="{Binding Age, Mode=TwoWay}" />
    </StackPanel>
</Window>

```

window中设置了UserViewModel,如果 `UserViewModel` 里有 `UserName`、`Age` 属性，两个控件会自动绑定，无需再写 `Source`。

#### 2.设置 DataContext 的方式

(1)  XAML静态设置

```xml
<Window.DataContext>
    <local:UserViewModel />
</Window.DataContext>

```

这种方式直接在 XAML 中实例化一个对象。子对象可直接调用(如上面[继承机制](#1.继承机制)中的示例)

(2) 代码设置

```c#
public MainWindow()
{
    InitializeComponent();
    this.DataContext = new UserViewModel();
}

```

将View 绑定到 ViewModel。

(3) 继承绑定

如果子元素没有设置 DataContext，它会自动用父元素的。

子控件也可以重新设置自己的 DataContext，这会覆盖继承的值。

```xml
<StackPanel DataContext="{StaticResource AnotherViewModel}">
    <TextBlock Text="{Binding Title}" />
</StackPanel>

```

这里的 `TextBlock` 会绑定 `AnotherViewModel.Title`，而不是窗口的 DataContext。

### 3.Binding(绑定)

> Binding类架起了控件和ViewModel之间的桥梁，它就像一个媒婆，指示了哪个控件的哪个属性与哪个ViewModel类的哪个属性建立绑定关系。提供对绑定定义的高级访问，该绑定连接绑定目标对象（通常为 WPF 元素）的属性和任何数据源（例如数据库、XML 文件，或包含数据的任何对象）。

#### 1.基本结构

```xml
{Binding Path=属性名, Source=数据源, Mode=绑定模式, UpdateSourceTrigger=更新触发时机, Converter=转换器}
```

<div align="center"> <strong>Binding数据源</strong></div>

| 数据源             | 写法                                                         | 用途                  |
| ------------------ | ------------------------------------------------------------ | --------------------- |
| **DataContext**    | `{Binding UserName}`                                         | 最常见，MVVM 默认用法 |
| **Source**         | `{Binding Source={StaticResource RedBrush}, Path=Color}`     | 指定对象作为数据源    |
| **ElementName**    | `{Binding ElementName=myTextBox, Path=Text}`                 | 绑定到另一个控件      |
| **RelativeSource** | `{Binding Path=ActualWidth, RelativeSource={RelativeSource AncestorType=Window}}` | 绑定到自己或祖先元素  |
| **x:Static**       | `{x:Static sys:DateTime.Now}`                                | 绑定静态属性/常量     |

```xaml
<StackPanel x:Name="panel" VerticalAlignment="Center" Margin="80,0">
<TextBlock Margin="5">
    <Run Text="Source示例:"/>
    <Run Text="{Binding Source={StaticResource RedBrush},Path=Color}"/>
</TextBlock>
<TextBlock Margin="5">
    <Run Text="ElementName示例:"/>
    <Run Text="{Binding ElementName=panel,Path=Margin}"/>
</TextBlock>
<TextBlock Margin="5">
    <Run Text="RelativeSource示例:"/>
    <Run Text="{Binding RelativeSource={RelativeSource Mode=Self},Path=Foreground}"/>
    <Run Text="{Binding RelativeSource={RelativeSource Mode=FindAncestor,AncestorType=StackPanel},Path=Margin}"/>
</TextBlock>
</StackPanel>
```



<div align="center"> <strong>Binding属性</strong></div>


|     名称     | 解释 |
| :--: | :----------: |
| Path | 数据源里的属性路径 |
| Source | 显式绑定的数据源（如果没写，就用 `DataContext`） |
| Mode | 绑定模式（枚举值:OneWay, TwoWay, OneTime, OneWayToSource）。 |
| UpdateSourceTrigger | 决定何时把 UI 的值写回源。也是一个枚举值<br />如果前端的值发生改变，后端的值在什么时候跟着改变。 |
| Converter | 值转换器（比如布尔值转颜色）。 |

<div align="center"><strong>Mode绑定模式</strong>  </div>

| 模式(枚举值) | 解释 |
| :-: | :--: |
| Default | 默认绑定，文本框的默认绑定是双向的，而其他大多数属性默认为单向绑定。 |
| TwoWay | 双向绑定，即导致更改源属性或目标属性时自动更新另一方。 |
| OneWay | 单向绑定，在更改绑定源（源）时更新绑定目标（目标）。 |
| OneTime | 一次绑定，在应用程序启动或数据上下文更改时，更新绑定目标。<br />适合显示不会变化的数据（比如常量）。 |
| OneWayToSource | 在目标属性更改时，更新源属性。源不影响UI |

<div align="center"><strong>UpdateSourceTrigger（更新触发时机）</strong>  </div>

|  时机(枚举值)   | 说明                                                         |
| :-------------: | ------------------------------------------------------------ |
|     Default     | 采用控件各自的UpdateSourceTrigger默认值。（TextBox.Text 默认是 `LostFocus`） |
| PropertyChanged | 每当绑定目标属性发生更改时，都会更新绑定源。                 |
|    LostFocus    | 每当绑定目标元素失去焦点时，都会更新绑定源。                 |
|    Explicit     | 仅在调用 System.Windows.Data.BindingExpression.UpdateSource 方法时更新绑定源。 |

#### 2.关于数据源RelativeSource



> 这种绑定方式经常使用，且实用价值很高

数据源中RelativeSource类有3个重要的属性，它们分别是Mode、AncestorType和AncestorLevel。

<div align="left"><strong>Mode</strong><br/>表示寻找相对数据源的模式，一共有4种模式  </div>

| 模式            | 说明                                                         |
| --------------- | ------------------------------------------------------------ |
| PreviousData    | 允许在当前显示的数据项列表中绑定上一个数据项（不是包含数据项的控件）。 |
| TemplatedParent | 引用应用了模板的元素，其中此模板中存在数据绑定元素。         |
| Self            | 引用正在其上设置绑定的元素，并允许你将该元素的一个属性绑定到同一元素的其他属性上。 |
| FindAncestor    | 引用数据绑定元素的父链中的上级。 这可用于绑定到特定类型的上级或其子类。 |

AncestorType：当Mode=FindAncestor时，这时就要指示要找的这个上级是什么类型，AncestorType用来表示上级的类型。

AncestorLevel：获取或设置要查找的上级级别。 使用 1 指示最靠近绑定目标元素的项。

---

#### 3.PATH(路径)


- **简单属性**

  ```
  <TextBlock Text="{Binding User.Name}" />
  ```

- **索引器**

  ```
  <TextBlock Text="{Binding Users[0].Name}" />
  ```

- **附加属性**

  ```
  <TextBlock Text="{Binding Path=(Grid.Row)}" />
  ```

#### 4.Converter（转换器）

> Binding类还有一个Converter属性，其实，它是一个IValueConverter接口。它的主要作用是：前后端建立绑定时，定义一套自定义逻辑，让前端显示的数据与后端获取的数据建立一定的对应关系。
>
> 比如Person对象有一个年龄（Age）属性，我们在前端显示某个人的年龄时，可以根据不同的年龄，显示不同的背景颜色。这个时候，实际上是根据这个输入的整型数据返回一个不同颜色的画刷。

##### 1.定义

``` c#
public interface IValueConverter
{
    object Convert(object value, Type targetType, object parameter, CultureInfo culture);
    object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture);
 
}
```

IValueConverter接口有两个方法成员，分别是Convert和ConvertBack。

Convert方法成员：输入的value及parameter参数，根据自定义逻辑判断，返回一个object对象给前端XAML使用。

ConvertBack方法成员：与Convert相反，将前端输入的数据转换成另一个对象返回给后端的数据源。

如果绑定的值和 UI 属性类型不一致，可以用 `IValueConverter` 转换。



##### 2.示例

首先在命名空间WPFStudy创建一个转换器:AgeToColorConverter,根据年龄来实现背景颜色变化

```C#
    public class AgeToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            SolidColorBrush background = Brushes.Black;
 
            if (value != null && int.TryParse(value.ToString(), out int age))
            {
                if (age < 20) { background = Brushes.Green; }
                else if (age < 40) { background = Brushes.Blue; }
                else if (age < 60) { background = Brushes.Orange; }
                else if (age < 80) { background = Brushes.Purple; }
                else if (age < 90) { background = Brushes.Red; }
            }
            return background;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
```

在前端中定义资源

```xaml
   <Window.Resources>
       <local:AgeToColorConverter x:Key="AgeToColorConverter"/>
   </Window.Resources>
```

在对应控件调用

```xaml
<TextBox Text="{Binding Person.Age,UpdateSourceTrigger=LostFocus}" Background="{Binding Person.Age,Converter={StaticResource AgeToColorConverter}}" Width="145" Height="25"/>
```

##### 3.IMultiValueConverter多值转换器

>与IValueConverter类似,但是可以传递多个值

在前端中选择传入的值Age和Money

```xaml
<TextBlock Margin="5" >
    <Run Text="称号:"/>
    <Run>
        <Run.Text>
            <MultiBinding Converter="{StaticResource MultiColorConverter}">
                <Binding Path="Person.Age" />
                <Binding Path="Person.Money"/>
            </MultiBinding>
        </Run.Text>
    </Run>                    
</TextBlock>
```

后端调用

```C#
 /// <summary>
 /// 多值转换器
 /// </summary>
 public class MultiColorConverter : IMultiValueConverter
 {
     public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
     {
         if (values != null && values.Length == 2)
         {
             var age_result = int.TryParse(values[0].ToString(), out int age);
             var money_result = int.TryParse(values[1].ToString(), out int money);
             if(age_result&& money_result)
             {
                 if (age < 30 && money > 50000)
                 {
                     return "年轻的有钱人";
                 }
                 else if (age >= 30 && age <= 60 && money < 5000)
                 {
                     return "没钱的中年人";
                 }
                 else if (age < 30 && money < 5000)
                 {
                     return "没钱的年轻人";
                 }
                 else if (age >= 30 && money > 90000)
                 {
                     return "有钱的中年人";
                 }
                 else
                 {
                     return "平凡的人";
                 }
             }
             
         }
         return null;
     }
 
     public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
     {
         throw new NotImplementedException();
     }
 }
```



#### 5.INotifyPropertyChanged 接口

> 当对象的某个属性值改变时，通知“Binding”属性更新了。

使用下面示例代码实现该接口,当Model中的Name改变时,通知UI中的Name进行改变

```C#
public class UserViewModel : INotifyPropertyChanged
{	
    //Model定义
    private string _name;
    public string Name
    {
        get => _name;
        set
        {
            if (_name != value)
            {
                _name = value;
                RaisePropertyChanged(nameof(Name));
            }
        }
    }
	//接口实现
    public event PropertyChangedEventHandler PropertyChanged;
	//传入属性名,告诉Binding引擎哪个属性改变了,需要刷新该属性
    protected void RaisePropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

```

使用[CallerMemberName]特性简化,自动传入属性名

```c#

protected void RaisePropertyChanged([CallerMemberName] string propertyName = "")
{
    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
}

```

通常情况下，可以单独编写一个服务类ObservableObject，以实现INotifyPropertyChanged接口的业务。这样做的好处是，将来的ViewModel、Model都可以继承这个ObservableObject，从而调用属性通知接口。

```c#
        public class ObservableObject : INotifyPropertyChanged
        {
            public event PropertyChangedEventHandler PropertyChanged;

            public void RaisePropertyChanged([CallerMemberName] string propertyName="")
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }
```

#### 6.ObservableCollection(泛型集合)

> 泛型集合类，继承自 `Collection<T>`。
>
> 实现了两个接口：
>
> - **`INotifyCollectionChanged`** → 通知集合项的增删改。
> - **`INotifyPropertyChanged`** → 通知集合属性的变化（比如 `Count` 改变）。

示例

前端:

``` xaml
<Window x:Class="WPFStudy.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
        xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
        xmlns:local="clr-namespace:WPFStudy"
        mc:Ignorable="d"
        Title="MyApp" Height="350" Width="500">
    <Grid>
        <Grid.ColumnDefinitions>
            <ColumnDefinition/>
            <ColumnDefinition Width="200"/>
        </Grid.ColumnDefinitions>
        <ListView ItemsSource="{Binding Persons}" SelectedItem="{Binding Person}">
            <ListView.View>
                <GridView>
                    <GridViewColumn Header="姓名" DisplayMemberBinding="{Binding Name}" Width="60"/>
                    <GridViewColumn Header="年龄" DisplayMemberBinding="{Binding Age}" Width="auto"/>
                    <GridViewColumn Header="地址" DisplayMemberBinding="{Binding Address}" Width="auto"/>
                </GridView>
            </ListView.View>
        </ListView>
        <StackPanel Grid.Column="1" x:Name="panel" VerticalAlignment="Center" Margin="5,0">
            <StackPanel Orientation="Horizontal">
                <TextBlock Text="姓名:" Margin="5"/>
                <TextBox Text="{Binding Person.Name,UpdateSourceTrigger=PropertyChanged}" Width="145" Height="25"/>
            </StackPanel>
            <StackPanel Orientation="Horizontal">
                <TextBlock Text="年龄:" Margin="5"/>
                <TextBox Text="{Binding Person.Age,UpdateSourceTrigger=LostFocus}" Width="145" Height="25"/>
            </StackPanel>
            <StackPanel Orientation="Horizontal">
                <TextBlock Text="地址:" Margin="5"/>
                <TextBox Text="{Binding Person.Address,UpdateSourceTrigger=Default}" Width="145" Height="25"/>
            </StackPanel>
            <TextBlock Margin="5" >
                <Run Text="姓名:"/>
                <Run Text="{Binding Person.Name}"/>
            </TextBlock>
            <TextBlock Margin="5" >
                <Run Text="年龄:"/>
                <Run Text="{Binding Person.Age}"/>
            </TextBlock>
            <TextBlock Margin="5" >
                <Run Text="住址:"/>
                <Run Text="{Binding Person.Address}"/>
            </TextBlock>
            <Button Content="增加用户" Click="Button_Click" Margin="5,0"/>
        </StackPanel>
    </Grid>
</Window>

```

后端:

```c#
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPFStudy
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {   

        public class ObservableObject : INotifyPropertyChanged
        {
            public event PropertyChangedEventHandler PropertyChanged;

            public void RaisePropertyChanged([CallerMemberName] string propertyName="")
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public class Person : ObservableObject
        {
            private string _Name;
            public string Name {
                get {return _Name; }
                set {_Name=value;RaisePropertyChanged(); } 
            }

            private int _Age;
            public int Age
            {
                get { return _Age; }
                set { _Age = value; RaisePropertyChanged(); }
            }

            private string _Address;
            public string Address
            {
                get { return _Address; }
                set { _Address = value; RaisePropertyChanged(); }
            }

        }
        
        public class MainViewModel : ObservableObject
        {
            private Person _Person;
            public Person Person
            {
                get { return _Person; }
                set { _Person = value; RaisePropertyChanged(); }

                }
            public MainViewModel()
            {
                _Person = new Person() { Name = "Aliex", Age = 20, Address = "Road 7" };
            }
        }

        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = new MainViewModel();
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var vm =DataContext as MainViewModel;
            if (vm == null) return;
            vm.Person.Age=new Random().Next(1,100);
            vm.Person.Address = DateTime.Now.ToString();

        }
    }
}

```



### 7.ValidationRule(验证规则)

> ValidationRule是一个抽象基类
>
> ``` C#
> public abstract class ValidationRule
> {
>     public abstract ValidationResult Validate(object value, CultureInfo cultureInfo);
> }
> ```
>
> 作用：在数据绑定过程中，对输入值进行验证（比如数字范围、非空、正则表达式等）。
>
> 如果验证失败，可以阻止绑定更新，并在 UI 上显示错误。

#### 1.验证输入年龄示例

限制输入的年龄必须是整数，且范围在 0~120 之间。

定义规则:

```C#
public class AgeValidationRule : ValidationRule
{
    public override ValidationResult Validate(object value, CultureInfo cultureInfo)
    {
        if (string.IsNullOrWhiteSpace(value as string))
            return new ValidationResult(false, "值不能为空");

        if (!int.TryParse(value.ToString(), out int age))
            return new ValidationResult(false, "必须是整数");

        if (age < 0 || age > 120)
            return new ValidationResult(false, "年龄必须在 0~120 之间");

        return ValidationResult.ValidResult; // 验证通过
    }
}

```

前端中调用,可以用Validation.ErrorTemplate自定义错误UI

```xaml
            <StackPanel Orientation="Horizontal">
                <TextBlock Text="年龄:" Margin="5"/>
                <TextBox Width="145" Height="25">
                    <TextBox.Text>
                        <Binding Path="Person.Age" UpdateSourceTrigger="PropertyChanged">
                            <Binding.ValidationRules>
                                <local:AgeValidationRule ValidatesOnTargetUpdated="True"/>
                            </Binding.ValidationRules>
                        </Binding>
                    </TextBox.Text>
                    <TextBox.Background>
                        <Binding Path="Person.Age" Converter="{StaticResource AgeToColorConverter}"/>
                    </TextBox.Background>
                    <Validation.ErrorTemplate>
                        <ControlTemplate>
                            <Grid>
                                <Grid.RowDefinitions>
                                    <RowDefinition Height="Auto"/>
                                    <RowDefinition Height="Auto"/>
                                </Grid.RowDefinitions>

                                <!-- 红框 -->
                                <Border Grid.Row="0" BorderBrush="Red" BorderThickness="1" CornerRadius="2">
                                    <AdornedElementPlaceholder x:Name="adorner" />
                                </Border>

                                <!-- 下方错误文本 -->
                                <TextBlock Grid.Row="1" Margin="0,4,0,0" Foreground="Red"
                   Text="{Binding ElementName=adorner,
                                  Path=AdornedElement.(Validation.Errors)[0].ErrorContent}"/>
                            </Grid>
                        </ControlTemplate>
                    </Validation.ErrorTemplate>
                    <TextBox.Style>
                        <Style TargetType="TextBox">
                            <Style.Triggers>
                                <Trigger Property="Validation.HasError" Value="True">
                                    <!-- 有错误文本时设置边缘来防止遮挡 -->
                                    <Setter Property="Margin" Value="0,0,0,22"/>
                                </Trigger>
                            </Style.Triggers>
                        </Style>
                    </TextBox.Style>
                </TextBox>
            </StackPanel>

```

或者使用布局+触发器的方式

```xaml
<StackPanel>
  <TextBox x:Name="tb" Width="145" Height="25"
           Text="{Binding Person.Age, UpdateSourceTrigger=PropertyChanged, 
                          ValidatesOnDataErrors=True}">
  </TextBox>

  <!-- 只在有错误时显示 -->
  <TextBlock Foreground="Red" Margin="0,4,0,0" Visibility="Collapsed"
             Text="{Binding ElementName=tb, 
                            Path=(Validation.Errors)[0].ErrorContent}">
    <TextBlock.Style>
      <Style TargetType="TextBlock">
        <Style.Triggers>
          <DataTrigger Binding="{Binding ElementName=tb, Path=(Validation.HasError)}" Value="True">
            <Setter Property="Visibility" Value="Visible"/>
          </DataTrigger>
        </Style.Triggers>
      </Style>
    </TextBlock.Style>
  </TextBlock>
</StackPanel>

```



# WPF样式

>WPF样式可以将一系列的属性的设置“集中”起来，将它们定义成一个样式，最后将这个样式再设置到控件上，从而达到“一处定义多处引用”。
>
>样式——就是一种将一组属性值应用到多个元素的便捷方法。







# XAML语法

> **XAML**：XML 语法的标记语言，用来**声明 UI 结构**（控件树、样式、资源…）。

### 1.基本结构与命名空间

```xaml
<Window x:Class="Demo.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"  <!-- WPF 控件 -->
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"             <!-- x: 前缀工具 -->
        xmlns:local="clr-namespace:Demo"                                   <!-- 本地类型 -->
        Title="My App" Width="400" Height="250">								<!-- 设置标题,窗口大小等属性 -->
						
<!--正文-->
    <Grid>
        <TextBlock Text="Hello XAML" FontSize="24" HorizontalAlignment="Center" VerticalAlignment="Center"/>
    </Grid>
</Window>

```

***1. 根元素<Window> ***

WPF 顶层窗口类型，继承 `ContentControl` → 只能有**一个根内容**

*** 2.`x:Class="Demo.MainWindow"`：把 XAML 绑定到代码幕后***

- **含义**：这个 XAML 对应到一个 **C# 部分类** `Demo.MainWindow`（`MainWindow.xaml.cs`）。
- **运行过程**：构建时 XAML 会编译成 **BAML**（二进制 XAML）嵌到程序资源里；构造函数里 `InitializeComponent()` 读取 BAML、创建控件树、设置属性。

- 改命名空间/类名时，`x:Class` 必须同步

*** 3.xmlns:local***

把项目里的 C# 命名空间 Demo 映射为前缀 local:以后可以直接在 XAML 里用你的类型：
 `<local:MyViewModel/>`、`<local:AgeValidationRule/>`           ←←←👈示例



### 2.属性设置

##### 1.特性(属性)语法(简写形式):

适用与简单场景

```xaml
<Button Content='确定' Width='100'/>
```

```xaml
<TextBox Text="{Binding Person.Age, UpdateSourceTrigger=LostFocus}" 
         Background="{Binding Person.Age, Converter={StaticResource AgeToColorConverter}}" 
         Width="145" Height="25"/>

```

#####　２.对象元素语法

> 规则：`<类型.属性> ... </类型.属性>`

用于复杂类型或需要嵌套

```xaml
<Button>
  <Button.Background>
    <LinearGradientBrush>
      <GradientStop Color="LightBlue" Offset="0"/>
      <GradientStop Color="SteelBlue"  Offset="1"/>
    </LinearGradientBrush>
  </Button.Background>
</Button>

```



```xaml
<TextBox Width="145" Height="25">
    <TextBox.Text>
        <Binding Path="Person.Age" UpdateSourceTrigger="PropertyChanged">
            <Binding.ValidationRules>
                <local:AgeValidationRule ValidatesOnTargetUpdated="True"/>
            </Binding.ValidationRules>
        </Binding>
    </TextBox.Text>
    <TextBox.Background>
        <Binding Path="Person.Age" Converter="{StaticResource AgeToColorConverter}"/>
    </TextBox.Background>
</TextBox>

```

### 3.附加属性

> 把某容器的定位/行为属性**附加**到子元素上
>
> 附加属性是一种 **特殊的依赖属性**，设计出来是为了让一个类可以把属性应用到其他对象上。
>
> 它就是“一个控件告诉另一个控件：你在我这里该怎么摆放/表现”。

##### 1.语法

```xaml
<父容器>
    <子控件 容器类.属性名="值" />
</父容器>
<!--格式-->
OwnerClass.PropertyName="value"
```

例如:

```xaml
<Grid>
    <Grid.RowDefinitions>
        <RowDefinition Height="Auto"/>
        <RowDefinition Height="*"/>
    </Grid.RowDefinitions>
    <Grid.ColumnDefinitions>
        <ColumnDefinition Width="2*"/>
        <ColumnDefinition Width="*"/>
    </Grid.ColumnDefinitions>

    <!-- 放在第 0 行第 0 列 -->
    <TextBlock Grid.Row="0" Grid.Column="0" Text="标题" />

    <!-- 放在第 1 行第 1 列 -->
    <Button Grid.Row="1" Grid.Column="1" Content="确定" />
</Grid>

```

`Grid.Row` 和 `Grid.Column` **不是 TextBlock 或 Button 自己的属性**，而是 **Grid 定义的附加属性**。

意思是：Grid 用这些属性来决定子元素的摆放位置。

### 4.标记扩展

> 标记扩展是 XAML 里用 **花括号 `{}`** 包裹的一种特殊语法，用来给属性赋值。
>
> **直白理解**：当你看到 `{ ... }`，它不是一个普通字符串，而是“动态求值”的入口,其实就是一个变量

##### 1.语法

```xaml
Property="{ExtensionName 参数1=值1, 参数2=值2}"
```

**Property** → 要赋值的属性，比如 Text。

**ExtensionName** → 标记扩展类名（省略了 “Extension” 后缀）。

**参数** → 传递给扩展的配置项。

常用扩展

```
{Binding}
{StaticResource}` / `{DynamicResource}
{x:Static}
{x:Type}
{RelativeSource}
{Binding ElementName=...}
```

# 相关概念

> 部分摘自微软的官方说明,对`WPF`中的相关概念作说明

---

***1.布局系统定义**： FrameworkElement 为中 UIElement定义为虚拟成员的某些方法提供特定的 WPF 框架级实现。 最值得注意的是， FrameworkElement 会密封某些 WPF 核心级布局替代，并改为提供派生类应替代的 WPF 框架级别的等效项。 例如，密封但 FrameworkElementArrangeCore 提供 ArrangeOverride。 这些更改反映了这样一个事实，即在 WPF 框架级别，有一个可以呈现任何 FrameworkElement 派生类的完整布局系统。 在 WPF 核心级别，将构建基于 WPF 的常规布局解决方案的某些成员已就位，但未定义布局系统的实际引擎。

**2.逻辑树**： 常规 WPF 编程模型通常以元素树的方式表示。 支持将元素树表示为逻辑树，以及支持在标记中定义该树的支持是在 级别实现的 FrameworkElement 。 但请注意， FrameworkElement 故意不定义内容模型，并将该责任留给派生类。

**3.对象生存期事件**： 了解何时初始化元素 (调用构造函数) 或首次将元素加载到逻辑树中时，这通常很有用。 FrameworkElement 定义多个与对象生存期相关的事件，这些事件为涉及元素的代码隐藏操作（例如添加更多子元素）提供有用的挂钩。

**4.支持数据绑定和动态资源引用**： 对数据绑定和资源的属性级支持由 DependencyProperty 类实现，并体现在属性系统中，但解析存储为 Expression (数据绑定和动态资源的编程构造) 中存储的成员值的能力由 FrameworkElement实现。

**5.风格**：FrameworkElement 定义 Style 属性。 但是， FrameworkElement 尚未定义对模板的支持或支持修饰器。 这些功能由控件类（如 和 ContentControl）Control引入。

**6.更多动画支持**： 某些动画支持已在 WPF 核心级别定义，但 FrameworkElement 通过实现 BeginStoryboard 和相关成员扩展了此支持。

**7.数据绑定**:数据绑定是在应用 UI 与其显示的数据之间建立连接的过程。 如果绑定具有正确的设置，并且数据提供适当的通知，则在数据更改其值时，绑定到该数据的元素会自动反映更改。 数据绑定还意味着，如果元素中数据的外部表示形式发生更改，则基础数据可以自动进行更新以反映更改。
